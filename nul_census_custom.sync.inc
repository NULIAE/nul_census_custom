<?php

/**
 * Check if params not empty and add them to be updated if they have values
 * @param  array $presetParams     params to be updated
 * @param  array $ifNotEmptyParams params to be added to be updated if they are not empty
 * @return array                  updated array of params
 */
function nul_census_custom_assemble_params($presetParams, $ifNotEmptyParams) {
  foreach ($ifNotEmptyParams as $key => $value) {
    if (!empty($value)) {
      $presetParams[$key] = $value;
    }
  }
  return $presetParams;
}

/**
 * Update Phone, Email or Address in civi based on Census Data in Drupal
 * @param  string $entity           Phone, Email, Address Etc.
 * @param  array $paramsToCheckOn   parameters to use to check for an exisiting record
 * @param  array $paramsToUpdateOn parameters to use to update the record
 */
function nul_census_custom_update_civicontact_info($entity, $paramsToCheckOn, $paramsToUpdateOn = array()) {
  $paramsToCheckOn['sequential'] = 1;
  $checkForExisting = nul_census_custom_civi_api($entity, 'get', $paramsToCheckOn);
  if ($checkForExisting['count'] >= 1 && !empty($checkForExisting['values'][0]['id'])) {
    $paramsToCheckOn['id'] = $checkForExisting['values'][0]['id'];
  }
  $paramsToUpdateOn = nul_census_custom_assemble_params($paramsToCheckOn, $paramsToUpdateOn);
  nul_census_custom_civi_api($entity, 'create', $paramsToUpdateOn);
}

/**
 * Find Affiliate in Civi based on Affiliate Select and dedupe rules see dedupe.md
 * @param  int $affiliateSelect affiliate select value
 * @return int                 civiContact Id if found else 0
 */
function nul_census_custom_find_affiliate($affiliateSelect) {
  $affiliateIdInCivi = 0;
  $params = array(
    'contact_type' => "Organization",
    'sequential' => 1,
    'contact_sub_type' => "Urban_League_Affiliate",
    'custom_296' => $affiliateSelect,
  );
  $civiAffiliate = nul_census_custom_civi_api('Contact', 'get', $params);
  if (!empty($civiAffiliate['count']) && $civiAffiliate['count'] == 0 || $civiAffiliate == array()) {
    watchdog('nul_census_custom', 'No matching Urban League Affiliate found please make sure there is a valid org to connect to', WATCHDOG_NOTICE, $link = NULL);
    drupal_set_message('No matching Urban League Affiliate found please make sure there is a valid org to connect to', 'error');
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
      watchdog('nul_census_custom', 'More than one matching Urban League Affiliate found please de dupe', array('%matchingAffiliates' => $civiAffiliate['values']), WATCHDOG_ERROR, $link = NULL);
      drupal_set_message('More than one matching Urban League Affiliate found please de dupe', 'error');
    }
  }
  if (!empty($civiAffiliate['count']) && $civiAffiliate['count'] == 1 && !empty($civiAffiliate['values'][0]['id'])) {
    $affiliateIdInCivi = $civiAffiliate['values'][0]['id'];
  }
  return $affiliateIdInCivi;
}

/**
 * Compile array of data to send to civi from tabs
 * @param  int $censusId      census id
 * @param  string $tabName       drupal content type machine name
 * @param  array $fieldMatching  array of fields in civi matched to their fields in drupal
 * @param  array $affiliateId  affiliate contactId in civi to be updated
 */
function nul_census_custom_get_tab_data($censusId, $tabName, $affiliateId, $fieldMatching) {
  // Array to store fields to be updated on the Affiliate
  $civiFieldsToUpdate = array(
    'id' => $affiliateId,
  );
  // Find the tab of this type for the census
  $tab = nul_census_custom_load_tab($tabName, $censusId);
  if (!empty($tab['node']) && key($tab['node']) > 0) {
    $tab = node_load(key($tab['node']));
  }
  // Compile Array of fields to update
  foreach ($fieldMatching as $civiFieldName => $drupalFieldName) {
    if (!empty($tab->{$drupalFieldName}[LANGUAGE_NONE][0]['value'])) {
      $civiFieldsToUpdate[$civiFieldName] = $tab->{$drupalFieldName}[LANGUAGE_NONE][0]['value'];
    }
  }
  nul_census_custom_civi_api('Contact', 'create', $civiFieldsToUpdate);
}

/**
 * Update Affiliate and Affiliate CEO contact information in Civi Based on Census in Drupal
 * @param  int $censusId nid of the census we are updating based off of
 */
function nul_census_custom_update_civi($censusId) {
  $census = node_load($censusId);
  // Find Affiliate
  if (!empty($census->field_affiliate_select[LANGUAGE_NONE][0]['value'])) {
    $affiliateIdInCivi = nul_census_custom_find_affiliate($census->field_affiliate_select[LANGUAGE_NONE][0]['value']);
    if ($affiliateIdInCivi > 0) {
      // Send Data from Community Relations tab to Civi
      nul_census_custom_get_tab_data($censusId, 'community_relations', $affiliateIdInCivi, array(
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
      ));

      // Send Data from Revenue tab to Civi
      nul_census_custom_get_tab_data($censusId, 'revenue', $affiliateIdInCivi, array(
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
      ));

      // TODO deal with programs (multiple)

      // TODO deal with Service Areas (multiple)
      // $serviceAreas = nul_census_custom_load_tab('service_areas', $censusId);
      // if (!empty($serviceAreas['node']) && key($serviceAreas['node']) > 0) {
      //   $serviceAreas = node_load(key($serviceAreas['node']));
      // }

      //TODO create fields to hold service area data (these are paragraphs in drupal so there will be more than one)
      // '' => $serviceAreas->field_service_area_population[LANGUAGE_NONE][0]['value'],
      // '' => $serviceAreas->field_service_area_city_county[LANGUAGE_NONE][0]['value'],
      // '' => $serviceAreas->field_service_area_white[LANGUAGE_NONE][0]['value'],
      // '' => $serviceAreas->field_service_area_hispanic[LANGUAGE_NONE][0]['value'],
      // '' => $serviceAreas->field_service_area_asian_am[LANGUAGE_NONE][0]['value'],
      // '' => $serviceAreas->field_service_area_native_am[LANGUAGE_NONE][0]['value'],
      // '' => $serviceAreas->field_service_area_african_am[LANGUAGE_NONE][0]['value'],
      // '' => $serviceAreas->field_service_area_other[LANGUAGE_NONE][0]['value'],

      // Update Affiliate Contact For fields on Census
      if (!empty($census->field_date_established[LANGUAGE_NONE][0]['value'])) {
        $civiOrgUpdate = nul_census_custom_civi_api('Contact', 'create', array(
          'custom_1069' => $census->field_date_established[LANGUAGE_NONE][0]['value'],
          'id' => $civiAffiliate['values'][0]['id'],
        ));
      }

      // Update Affiliate Address
      $orgAddressParamsToCheck = array(
        'sequential' => 1,
        'contact_id' => $civiAffiliate['values'][0]['id'],
        'location_type_id' => "Main",
      );
      $orgAddressParamsToUpdate = array(
        'street_address' => $census->field_address_line_1[LANGUAGE_NONE][0]['value'],
        'supplemental_address_1' => $census->field_address_line_2[LANGUAGE_NONE][0]['value'],
        'city' => $census->field_city[LANGUAGE_NONE][0]['value'],
        'state_province_id' => $census->field_state_province[LANGUAGE_NONE][0]['value'],
        // 'postal_code' => $census->field_zip_code[LANGUAGE_NONE][0]['value'],
      );
      // $orgAddress = nul_census_custom_assemble_params(array(), $orgAddressParams);
      nul_census_custom_update_civicontact_info('Address', $orgAddressParamsToCheck, $orgAddressParamsToUpdate);

      // Update Org Telephone
      if (!empty($census->field_telephone[LANGUAGE_NONE][0]['value'])) {
        $telephoneCheck = array(
          'contact_id' => $civiAffiliate['values'][0]['id'],
          'location_type_id' => "Main",
          'phone_type_id' => "Phone",
        );
        $telephoneUpdate = array(
          'phone' => $census->field_telephone[LANGUAGE_NONE][0]['value'],
        );
        nul_census_custom_update_civicontact_info('Phone', $telephoneCheck, $telephoneUpdate);
      }

      // Update Org Fax
      if (!empty($census->field_fax[LANGUAGE_NONE][0]['value'])) {
        $faxCheck = array(
          'contact_id' => $civiAffiliate['values'][0]['id'],
          'location_type_id' => "Main",
          'phone_type_id' => "Fax",
        );
        $faxUpdate = array(
          'phone' => $census->field_fax[LANGUAGE_NONE][0]['value'],
        );
        nul_census_custom_update_civicontact_info('Phone', $faxCheck, $faxUpdate);
      }

      // Find Affiliate CEO
      $civiContactId = nul_census_custom_find_affiliate_ceo(array(
        'contact_type' => "Individual",
        'sequential' => 1,
        'first_name' => $census->field_president_ceo_first_name[LANGUAGE_NONE][0]['value'],
        'last_name' => $census->field_president_ceo_last_name[LANGUAGE_NONE][0]['value'],
        'custom_1014' => "Affiliate CEO",
      ));

      // If contact found
      if ($civiContactId > 0) {
        // Update contact details
        $fieldsToUpdateOnContact = array(
          'middle_name' => $census->field_president_ceo_middle_name[LANGUAGE_NONE][0]['value'],
          'image_URL' => $census->field_photo[LANGUAGE_NONE][0]['uri'],
          'id' => $civiContactId,
          'custom_296' => $census->field_affiliate_select[LANGUAGE_NONE][0]['value'],
          'custom_1070' => $census->field_number_of_years_as_ceo[LANGUAGE_NONE][0]['value'],
          'custom_1071' => $census->field_number_of_years_of_service[LANGUAGE_NONE][0]['value'],
        );
        $civiContactUpdate = nul_census_custom_civi_api('Contact', 'create', $fieldsToUpdateOnContact);

        // Update Cell Phone
        if (!empty($census->field_cellular_number[LANGUAGE_NONE][0]['value'])) {
          $cellCheck = array(
            'contact_id' => $civiContactId,
            'location_type_id' => "Main",
            'phone_type_id' => "Mobile",
          );
          $cellUpdate = array(
            'phone' => $census->field_cellular_number[LANGUAGE_NONE][0]['value'],
          );
          $civiContactCell = nul_census_custom_update_civicontact_info('Phone', $cellCheck, $cellUpdate);
        }
        // Update Email
        if (!empty($census->field_email_address[LANGUAGE_NONE][0]['value'])) {
          $emailCheck = array(
            'contact_id' => $civiContactId,
            'email' => $census->field_email_address[LANGUAGE_NONE][0]['value'],
            'location_type_id' => "Main",
          );
          $emailUpdate = array(
            'email' => $census->field_email_address[LANGUAGE_NONE][0]['value'],
          );
          $civiContactEmail = nul_census_custom_update_civicontact_info('Email', $emailCheck, $emailUpdate);
        }
        // Update Relationship
        // check for relationship and create if necessary
        $civiRelationship = nul_census_custom_update_civicontact_info('Relationship', array(
          'contact_id_b' => $civiAffiliate['values'][0]['id'],
          'contact_id_a' => $civiContactId,
          'relationship_type_id' => 5,
        ));
      }
    }
  }
  else {
    watchdog('nul_census_custom', 'No Affiliate Selected on Census so we cannot sync to Civi', WATCHDOG_ERROR, $link = NULL);
  }
}

/**
 * Find Affiliate CEO
 * @param  array $checkForIndiv params to search on
 * @return int              CEO civi Contact ID
 */
function nul_census_custom_find_affiliate_ceo($checkForIndiv) {
  $civiContact = nul_census_custom_civi_api('Contact', 'get', $checkForIndiv);
  $civiContactId = 0;
  // IF only one contact found update it
  if ($civiContact['count'] == 1) {
    $civiContactId = $civiContact['values'][0]['id'];
  }
  // IF no contact found throw error
  if ($civiContact['count'] == 0) {
    watchdog('nul_census_custom', 'No matching contact found for Affiliate CEO', array('%matchingAffiliates' => $civiAffiliate['values']), WATCHDOG_ERROR, $link = NULL);
    drupal_set_message('No matching CEO contact found for this Urban League Affiliate found please', 'error');
  }
  if ($civiContact['count'] > 1) {
    watchdog('nul_census_custom', 'More than one matching Contact found please de dupe', array('%matchingAffiliates' => $civiAffiliate['values']), WATCHDOG_ERROR, $link = NULL);
    drupal_set_message('More than one matching contact found for this Urban League Affiliate found please de dupe', 'error');
  }
  return $civiContactId;
}
