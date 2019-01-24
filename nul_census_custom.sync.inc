<?php

/**
 * Update Phone, Email or Address in civi based on Census Data in Drupal
 * @param  string $entity           Phone, Email, Address Etc.
 * @param  array $paramsToCheckOn   parameters to use to check for an exisiting record
 * @param  array $paramsToUpdateOn parameters to use to update the record
 */
function nul_census_custom_update_civicontact_info($census, $entity, $paramsToCheckOn, $paramsToUpdateOn = array()) {
  $paramsToCheckOn['sequential'] = 1;
  $checkForExisting = nul_census_custom_civi_api($entity, 'get', $paramsToCheckOn);
  if ($checkForExisting['count'] >= 1 && !empty($checkForExisting['values'][0]['id'])) {
    $paramsToCheckOn['id'] = $checkForExisting['values'][0]['id'];
  }
  if (!empty($paramsToUpdateOn)) {
    foreach ($paramsToUpdateOn as $type => $drupalField) {
      if (!empty($census->{$drupalField}[LANGUAGE_NONE][0]['value'])) {
        $paramsToUpdateOn[$type] = $census->{$drupalField}[LANGUAGE_NONE][0]['value'];
      }
      else {
        $paramsToUpdateOn[$type] = '';
      }
    }
  }
  $paramsToUpdateOn = array_merge($paramsToCheckOn, $paramsToUpdateOn);
  nul_census_custom_civi_api($entity, 'create', $paramsToUpdateOn);
}

/**
 * Find Affiliate in Civi based on Affiliate Select and dedupe rules see dedupe.md
 * @param  int $affiliateSelect affiliate select value
 * @return int                 civiContact Id if found else 0
 */
function nul_census_custom_find_affiliate($affiliateSelect, $censusId = NULL) {
  $affiliateIdInCivi = 0;
  $params = array(
    'contact_type' => "Organization",
    'sequential' => 1,
    'contact_sub_type' => "Urban_League_Affiliate",
    'custom_296' => $affiliateSelect,
  );
  $civiAffiliate = nul_census_custom_civi_api('Contact', 'get', $params);
  if (!empty($civiAffiliate['count']) && $civiAffiliate['count'] == 0 || $civiAffiliate == array()) {
    drupal_set_message(t('No Affiliate Organization found in civi for Census nid <a href="/node/!nid">!nid</a>', array(
      '!nid' => $censusId,
    )), 'warning');
  }
  if (!empty($civiAffiliate['count']) && $civiAffiliate['count'] > 1) {
    // IF more than one affiliate found check which is in the ACL group
    foreach ($civiAffiliate['values'] as $key => $value) {
      $aclParams = array(
        'group_id' => "Urban_League_Affiliate_19",
        'contact_id' => $value['contact_id'],
        'status' => 'added',
      );
      $checkACL = nul_census_custom_civi_api('GroupContact', 'get', $aclParams);
      if ($checkACL['count'] == 1) {
        $affiliateIdInCivi = $value['contact_id'];
      }
    }
    // if still no affiliate id throw error
    if ($affiliateIdInCivi == 0) {
      drupal_set_message(t('More than one matching Urban League Affiliate found in civi for Census nid <a href="/node/!nid">!nid</a>', array(
        '!nid' => $censusId,
      )), 'warning');
    }
  }
  if (!empty($civiAffiliate['count']) && $civiAffiliate['count'] == 1 && !empty($civiAffiliate['values'][0]['id'])) {
    $affiliateIdInCivi = $civiAffiliate['values'][0]['id'];
  }
  return $affiliateIdInCivi;
}
function nul_census_custom_clear_out_custom_values($affiliateId, $customFieldInGroup, $customGroupId) {
  $existing = nul_census_custom_civi_api('CustomValue', 'get', array(
    'sequential' => 1,
    'entity_id' => $affiliateId,
    $customFieldInGroup => 1,
  ));
  if (!empty($existing['values'][0])) {
    foreach ($existing['values'][0] as $key => $value) {
      if (is_numeric($key)) {
        CRM_Core_BAO_CustomValue::deleteCustomValue($key, $customGroupId);
      }
    }
  }
}

/**
 * Compile array of data to send to civi from tabs
 * @param  int $censusId      census id
 * @param  string $tabName       drupal content type machine name
 * @param  int $affiliateId  affiliate contactId in civi to be updated
 * @param  array $fieldMatching  array of fields in civi matched to their fields in drupal
 */
function nul_census_custom_get_tab_data($censusId, $tabName, $affiliateId, $fieldMatching) {
  switch ($tabName) {
    case 'programs':
      // Clear out existing data
      nul_census_custom_clear_out_custom_values($affiliateId, 'return.custom_1080', 140);

      $programs = nul_census_custom_load_tab($tabName, $censusId);
      if (!empty($programs['node']) && count($programs['node']) >= 1) {
        foreach ($programs['node'] as $nid => $otherInfo) {
          $program = node_load($nid);
          $programDataToSendToCivi = array(
            'entity_id' => $affiliateId,
          );
          foreach ($fieldMatching as $civiFieldName => $drupalFieldName) {
            switch ($drupalFieldName) {
              case 'title':
                if (!empty($program->{$drupalFieldName})) {
                  $programDataToSendToCivi[$civiFieldName] = $program->{$drupalFieldName};
                }
                break;

              case 'field_program_area':
                if (!empty($program->{$drupalFieldName}[LANGUAGE_NONE][0]['tid'])) {
                  $taxonomy_term = taxonomy_term_load($program->{$drupalFieldName}[LANGUAGE_NONE][0]['tid']);
                  if (!empty($taxonomy_term->name)) {
                    $programDataToSendToCivi[$civiFieldName] = $taxonomy_term->name;
                  }
                }
                break;

              default:
                if (!empty($program->{$drupalFieldName}[LANGUAGE_NONE][0])) {
                  $programDataToSendToCivi[$civiFieldName] = $program->{$drupalFieldName}[LANGUAGE_NONE][0]['value'];
                }
                break;
            }
          }
          nul_census_custom_civi_api('CustomValue', 'create', $programDataToSendToCivi);
        }
      }
      break;

    case 'service_areas':
      // Find the tab of this type for the census
      $tab = nul_census_custom_load_tab($tabName, $censusId);
      if (!empty($tab['node']) && key($tab['node']) > 0 && count($tab['node']) == 1) {
        $tab = node_load(key($tab['node']));
      }
      // make sure to clear out existing data
      nul_census_custom_clear_out_custom_values($affiliateId, 'return.custom_1073', 139);

      if (!empty($tab->field_service_areas[LANGUAGE_NONE])) {
        foreach ($tab->field_service_areas[LANGUAGE_NONE] as $key => $paragraphInfo) {
          $paragraphParamsToSendToCivi = array(
            'entity_id' => $affiliateId,
          );
          $paragraph = entity_load('paragraphs_item', array($paragraphInfo['value']));
          foreach ($fieldMatching as $civiFieldName => $drupalFieldName) {
            if (!empty($paragraph[$paragraphInfo['value']]->{$drupalFieldName}[LANGUAGE_NONE][0]['value'])) {
              $paragraphParamsToSendToCivi[$civiFieldName] = $paragraph[$paragraphInfo['value']]->{$drupalFieldName}[LANGUAGE_NONE][0]['value'];
            }
          }
          nul_census_custom_civi_api('CustomValue', 'create', $paragraphParamsToSendToCivi);
        }
      }
      break;

    default:
      // Find the tab of this type for the census
      $tab = nul_census_custom_load_tab($tabName, $censusId);
      if (!empty($tab['node']) && key($tab['node']) > 0 && count($tab['node']) == 1) {
        $tab = node_load(key($tab['node']));
      }
      // Array to store fields to be updated on the Affiliate
      $civiFieldsToUpdate = array(
        'id' => $affiliateId,
      );
      // Compile Array of fields to update
      foreach ($fieldMatching as $civiFieldName => $drupalFieldName) {
        if (!empty($tab->{$drupalFieldName}[LANGUAGE_NONE][0])) {
          $civiFieldsToUpdate[$civiFieldName] = $tab->{$drupalFieldName}[LANGUAGE_NONE][0]['value'];
        }
        else {
          $civiFieldsToUpdate[$civiFieldName] = '';
        }
      }
      nul_census_custom_civi_api('Contact', 'create', $civiFieldsToUpdate);

      break;
  }
}

function nul_census_custom_fieldmapping($tab) {
  switch ($tab) {
    case 'census':
      $mapping = array(
        // Fields on the CEO
        'CEO' => array(
          'field_president_ceo_first_name' => 'first_name',
          'field_president_ceo_middle_name' => 'middle_name',
          'field_president_ceo_last_name' => 'last_name',
          'field_number_of_years_as_ceo' => 'custom_1070',
          'field_number_of_years_of_service' => 'custom_1071',
          'field_photo' => 'image_URL',
        ),

        // Email/Phones
        'contact' => array(
          // CEO Contact Info
          'field_cellular_number' => array(
            'entity' => 'phone',
            'phone_type_id' => 'phone',
            'location' => 'mobile',
            'contact' => 'CEO',
          ),
          'field_email_address' => array(
            'entity' => 'email',
            'location' => 'main',
            'contact' => 'CEO',
          ),
          'field_telephone' => array(
            'entity' => 'phone',
            'phone_type_id' => 'phone',
            'location' => 'main',
            'contact' => 'org',
          ),
          'field_fax' => array(
            'entity' => 'phone',
            'phone_type_id' => 'fax',
            'location' => 'main',
            'contact' => 'org',
          ),
        ),

        // Fields On the Org
        'org' => array(
          'field_affiliate_select' => 'id',
          'field_date_established' => 'custom_1069',
        ),

        // Org Address Fields
        'address' => array(
          'field_address_line_1' => 'street_address',
          'field_address_line_2' => 'supplemental_address_1',
          'field_city' => 'city',
          'field_state_province' => 'state_province_id',
          'field_zip_code' => 'postal_code',
        ),

      );

      break;

    case 'community-relations':
      $mapping = array(
        'custom_1084' => 'field_produces_annual_report',
        'custom_1085' => 'field_newsletter',
        'custom_1086' => 'field_state_of_black_report',
        'custom_1087' => 'field_maintains_website',
        'custom_1089' => 'field_affiliate_website_address',
        'custom_1090' => 'field_monthly_website_hits',
        'custom_1091' => 'field_is_website_linked_to_nul',
        'custom_1092' => 'field_produces_tv_or_cable_show',
        'custom_1093' => 'field_produces_a_radio_show',
        'custom_1094' => 'field_has_ad_marketing_campaign',
        'custom_1096' => 'field_method_of_ad_marketing',
        'custom_1095' => 'field_marketing_kit_or_pamphlet',
      );
      break;

    case 'revenue':
      $mapping = array(
        'custom_1098' => 'field_revenue_investment',
        'custom_1099' => 'field_revenue_corporations',
        'custom_1100' => 'field_revenue_foundations',
        'custom_1101' => 'field_revenue_individual_members',
        'custom_1102' => 'field_revenue_special_events',
        'custom_1103' => 'field_revenue_united_way',
        'custom_1106' => 'field_revenue_federal',
        'custom_1105' => 'field_revenue_state_local',
        'custom_1107' => 'field_revenue_nul',
        'custom_1108' => 'field_revenue_purpose_of_funding',
        'custom_1109' => 'field_revenue_other',
        'custom_1112' => 'field_revenue_total_budget',
        'custom_1113' => 'field_revenue_has_endowment',
        'custom_1111' => 'field_revenue_endowment_amount',
      );
      break;

    case 'programs':
      $mapping = array(
        'custom_1080' => 'title',
        'custom_1081' => 'field_program_budget',
        'custom_1082' => 'field_program_served_total',
        'custom_1083' => 'field_program_area',
        'custom_1097' => 'field_program_nul_funded',
      );
      break;

    case 'service_areas':
      $mapping = array(
        'custom_1073' => 'field_service_area_population',
        'custom_1072' => 'field_service_area_city_county',
        'custom_1074' => 'field_service_area_white',
        'custom_1075' => 'field_service_area_hispanic',
        'custom_1076' => 'field_service_area_asian_am',
        'custom_1077' => 'field_service_area_native_am',
        'custom_1078' => 'field_service_area_african_am',
        'custom_1079' => 'field_service_area_other',
      );
      break;

    default:
      $mapping = array();
      break;
  }
  return $mapping;
}
/**
 * Update Affiliate and Affiliate CEO contact information in Civi Based on Census in Drupal
 * @param  int $censusId nid of the census we are updating based off of
 */
function nul_census_custom_update_civi($censusId) {
  $census = node_load($censusId);
  // Find Affiliate
  if (!empty($census->field_affiliate_select[LANGUAGE_NONE][0]['value'])) {
    $affiliateIdInCivi = nul_census_custom_find_affiliate($census->field_affiliate_select[LANGUAGE_NONE][0]['value'], $censusId);
    if ($affiliateIdInCivi > 0) {
      $tabsWithBasicFields = array(
        'community_relations',
        'revenue',
        'service_areas',
        'programs',
      );
      foreach ($tabsWithBasicFields as $key => $tab) {
        // TODO deal with programs (multiple)
        // TODO deal with service areas (multiple)
        $mapping = nul_census_custom_fieldmapping($tab);
        nul_census_custom_get_tab_data($censusId, $tab, $affiliateIdInCivi, $mapping);
      }

      // Update Affiliate Contact For fields on Census
      if (!empty($census->field_date_established[LANGUAGE_NONE][0]['value'])) {
        $civiOrgUpdate = nul_census_custom_civi_api('Contact', 'create', array(
          'custom_1069' => $census->field_date_established[LANGUAGE_NONE][0]['value'],
          'id' => $affiliateIdInCivi,
        ));
      }

      // Update Affiliate Address
      $orgAddressParamsToCheck = array(
        'sequential' => 1,
        'contact_id' => $affiliateIdInCivi,
        'location_type_id' => "Main",
      );

      $orgAddressParamsToUpdate = array(
        'street_address' => 'field_address_line_1',
        'supplemental_address_1' => 'field_address_line_2',
        'city' => 'field_city',
        'state_province_id' => 'field_state_province',
        'postal_code' => 'field_zip_code',
      );
      // $orgAddressParamsToUpdate = nul_census_custom_compile_api_params($orgAddressParams, $census);

      nul_census_custom_update_civicontact_info($census, 'Address', $orgAddressParamsToCheck, $orgAddressParamsToUpdate);

      // Update Org Telephone
      if (!empty($census->field_telephone[LANGUAGE_NONE][0]['value'])) {
        $telephoneCheck = array(
          'contact_id' => $affiliateIdInCivi,
          'location_type_id' => "Main",
          'phone_type_id' => "Phone",
        );
        $telephoneUpdate = array(
          'phone' => 'field_telephone',
        );
        nul_census_custom_update_civicontact_info($census, 'Phone', $telephoneCheck, $telephoneUpdate);
      }

      // Update Org Fax
      if (!empty($census->field_fax[LANGUAGE_NONE][0]['value'])) {
        $faxCheck = array(
          'contact_id' => $affiliateIdInCivi,
          'location_type_id' => "Main",
          'phone_type_id' => "Fax",
        );
        $faxUpdate = array(
          'phone' => 'field_fax',
        );
        nul_census_custom_update_civicontact_info($census, 'Phone', $faxCheck, $faxUpdate);
      }

      // Find Affiliate CEO
      $paramsToAddIfPopulated = array(
        'first_name' => 'field_president_ceo_first_name',
        'last_name' => 'field_president_ceo_last_name',
      );

      $paramsToSearchForCEO = nul_census_custom_compile_api_params($paramsToAddIfPopulated, $census, array(
        'contact_type' => "Individual",
        'sequential' => 1,
        'custom_1014' => "Affiliate CEO",
      ));

      $civiContactId = nul_census_custom_find_affiliate_ceo($paramsToSearchForCEO, $censusId);

      // If contact found
      if ($civiContactId > 0) {
        // Update contact details
        $fieldsKeyed = array(
          'middle_name' => 'field_president_ceo_middle_name',
          'custom_296' => 'field_affiliate_select',
          'custom_1070' => 'field_number_of_years_as_ceo',
          'custom_1071' => 'field_number_of_years_of_service',
        );
        $fieldsToUpdateOnContact = nul_census_custom_compile_api_params($fieldsKeyed, $census, array(
          'id' => $civiContactId,
        ));
        if (!empty(file_create_url($census->field_photo[LANGUAGE_NONE][0]['uri']))) {
          $fieldsToUpdateOnContact['image_URL'] = file_create_url($census->field_photo[LANGUAGE_NONE][0]['uri']);
        }
        $civiContactUpdate = nul_census_custom_civi_api('Contact', 'create', $fieldsToUpdateOnContact);

        // Update Cell Phone
        if (!empty($census->field_cellular_number[LANGUAGE_NONE][0]['value'])) {
          $cellCheck = array(
            'contact_id' => $civiContactId,
            'location_type_id' => "Main",
            'phone_type_id' => "Mobile",
          );
          $cellUpdate = array(
            'phone' => 'field_cellular_number',
          );
          $civiContactCell = nul_census_custom_update_civicontact_info($census, 'Phone', $cellCheck, $cellUpdate);
        }
        // Update Email
        if (!empty($census->field_email_address[LANGUAGE_NONE][0]['value'])) {
          $emailCheck = array(
            'contact_id' => $civiContactId,
            'email' => $census->field_email_address[LANGUAGE_NONE][0]['value'],
            'location_type_id' => "Main",
          );
          $emailUpdate = array(
            'email' => 'field_email_address',
          );
          $civiContactEmail = nul_census_custom_update_civicontact_info($census, 'Email', $emailCheck, $emailUpdate);
        }
        // Update Relationship
        // check for relationship and create if necessary
        $civiRelationship = nul_census_custom_update_civicontact_info($census, 'Relationship', array(
          'contact_id_b' => $affiliateIdInCivi,
          'contact_id_a' => $civiContactId,
          'relationship_type_id' => 5,
        ));
        drupal_set_message(t('Census <a href="/node/!nid">!nid</a> synced to Affiliate CEO civi id <a href="/civicrm/contact/view?reset=1&cid=!ceo">!ceo</a>', array(
          '!nid' => $censusId,
          '!ceo' => $civiContactId,
        )));
      }
      drupal_set_message(t('Census <a href="/node/!nid">!nid</a> synced to Affiliate ORG id <a href="/civicrm/contact/view?reset=1&cid=!civicid">!civicid</a>', array(
        '!nid' => $censusId,
        '!civicid' => $affiliateIdInCivi,
      )));
    }
  }
  else {
    drupal_set_message(t('No Affiliate Organization found in civi for Census nid <a href="/node/!nid">!nid</a>', array(
      '!nid' => $censusId,
    )), 'warning');
  }
}

/**
 * Find Affiliate CEO
 * @param  array $checkForIndiv params to search on
 * @return int              CEO civi Contact ID
 */
function nul_census_custom_find_affiliate_ceo($checkForIndiv, $censusId) {
  $civiContact = nul_census_custom_civi_api('Contact', 'get', $checkForIndiv);
  $civiContactId = 0;
  // IF only one contact found update it
  if ($civiContact['count'] == 1) {
    $civiContactId = $civiContact['values'][0]['id'];
  }
  // IF no contact found throw error
  if ($civiContact['count'] == 0) {
    drupal_set_message(t('No Affiliate CEO found in civi for Census nid <a href="/node/!nid">!nid</a>', array(
      '!nid' => $censusId,
    )), 'warning');
  }
  if ($civiContact['count'] > 1) {
    drupal_set_message(t('More than one matching Affiliate CEO Contact found Census nid <a href="/node/!nid">!nid</a>', array(
      '!nid' => $censusId,
    )), 'warning');
  }
  return $civiContactId;
}

/**
 * Form to sync all censuses for a given year to civi (/civisync)
 */
function nul_census_custom_sync_form($form, &$form_state) {
  $year = date('Y');
  $yearFieldInfo = field_info_field('field_year');
  $form['field_year'] = array(
    '#title' => 'Year',
    '#type' => 'select',
    '#default_value' => $year,
    '#options' => $yearFieldInfo['settings']['allowed_values'],
    '#required' => TRUE,
  );
  // status_type_for_census vocab
  $censusStatusVocabulary = taxonomy_vocabulary_machine_name_load('status_type_for_census');
  $censusTabs = taxonomy_get_tree($censusStatusVocabulary->vid);
  $statusOptions = array();
  foreach ($censusTabs as $key => $statusInfo) {
    $statusOptions[$statusInfo->tid] = $statusInfo->name;
  }
  $form['field_census_status'] = array(
    '#title' => 'Census Status',
    '#type' => 'select',
    '#options' => $statusOptions,
    '#required' => TRUE,
  );
  $defaultValue = array_search('Reviewed Complete', $statusOptions);
  if ($defaultValue > 0) {
    $form['field_census_status']['#default_value'] = $defaultValue;
  }

  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

function nul_census_custom_sync_form_submit($form, &$form_state) {
  if (!empty($form['field_year']['#value']) && !empty($form['field_census_status']['#value'])) {
    // Get all censuses for a given year
    $query = new EntityFieldQuery();
    $results = $query->entityCondition('entity_type', 'node')
      ->entityCondition('bundle', 'census')
      ->fieldCondition('field_year', 'value', $form['field_year']['#value'])
      ->fieldCondition('field_census_status', 'tid', $form['field_census_status']['#value'])
      ->propertyCondition('status', NODE_PUBLISHED)
      ->execute();
    if (!empty($results['node'])) {
      foreach ($results['node'] as $censusId => $values) {
        if ($censusId > 0) {
          nul_census_custom_update_civi($censusId);
        }
      }
    }
  }
}

function nul_census_custom_compile_api_params($civiFieldsMatchedToDrupal, $drupalData, $arrayToAddTo = array()) {
  foreach ($civiFieldsMatchedToDrupal as $civiField => $drupalField) {
    if (!empty($drupalData->{$drupalField}[LANGUAGE_NONE][0]['value'])) {
      $arrayToAddTo[$civiField] = $drupalData->{$drupalField}[LANGUAGE_NONE][0]['value'];
    }
  }
  return $arrayToAddTo;
}
