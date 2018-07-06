<?php

function nul_census_custom_assemble_params($presetParams, $ifNotEmptyParams) {
  foreach ($ifNotEmptyParams as $key => $value) {
    if (!empty($value)) {
      $presetParams[$key] = $value;
    }
  }
  return $presetParams;
}
function nul_census_custom_update_civicontact_info($entity, $paramsToCheckOn, $paramsToUpdateOn = array()) {
  $paramsToCheckOn['sequential'] = 1;
  $checkForExisting = nul_census_custom_civi_api($entity, 'get', $paramsToCheckOn);
  if ($checkForExisting['count'] >= 1 && !empty($checkForExisting['values'][0]['id'])) {
    $paramsToCheckOn['id'] = $checkForExisting['values'][0]['id'];
  }
  $paramsToUpdateOn = nul_census_custom_assemble_params($paramsToCheckOn, $paramsToUpdateOn);
  nul_census_custom_civi_api($entity, 'create', $paramsToUpdateOn);
}

function nul_census_custom_update_civi($censusId) {
  $census = node_load($censusId);
  nul_census_custom_find_affiliate();
  // Find Affiliate
  $affiliateIdInCivi = 0;
  if (!empty($census->field_affiliate_select[LANGUAGE_NONE][0]['value'])) {
    $params = array(
      'contact_type' => "Organization",
      'sequential' => 1,
      'contact_sub_type' => "Urban_League_Affiliate",
      'custom_296' => $census->field_affiliate_select[LANGUAGE_NONE][0]['value'],
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
    if ($affiliateIdInCivi > 0) {
      nul_census_custom_get_tab_data();
      // Find Community Relations Tab
      $comRel = nul_census_custom_load_tab('community_relations', $censusId);
      if (!empty($comRel['node']) && key($comRel['node']) > 0) {
        $comRel = node_load(key($comRel['node']));
      }

      // Find Service Areas Tab
      $serviceAreas = nul_census_custom_load_tab('service_areas', $censusId);
      if (!empty($serviceAreas['node']) && key($serviceAreas['node']) > 0) {
        $serviceAreas = node_load(key($serviceAreas['node']));
      }

      // Find Revenue Tab
      $revenue = nul_census_custom_load_tab('revenue', $censusId);
      if (!empty($revenue['node']) && key($revenue['node']) > 0) {
        $revenue = node_load(key($revenue['node']));
      }

      //TODO create fields to hold service area data (these are paragraphs in drupal so there will be more than one)
      // '' => $serviceAreas->field_service_area_population[LANGUAGE_NONE][0]['value'],
      // '' => $serviceAreas->field_service_area_city_county[LANGUAGE_NONE][0]['value'],
      // '' => $serviceAreas->field_service_area_white[LANGUAGE_NONE][0]['value'],
      // '' => $serviceAreas->field_service_area_hispanic[LANGUAGE_NONE][0]['value'],
      // '' => $serviceAreas->field_service_area_asian_am[LANGUAGE_NONE][0]['value'],
      // '' => $serviceAreas->field_service_area_native_am[LANGUAGE_NONE][0]['value'],
      // '' => $serviceAreas->field_service_area_african_am[LANGUAGE_NONE][0]['value'],
      // '' => $serviceAreas->field_service_area_other[LANGUAGE_NONE][0]['value'],

      // Update Affiliate Contact
      if (!empty($census->field_date_established[LANGUAGE_NONE][0]['value'])) {
        $civiOrgUpdate = nul_census_custom_civi_api('Contact', 'create', array(
          'custom_1069' => $census->field_date_established[LANGUAGE_NONE][0]['value'],
          // TODO create custom fields to hold Community Relations
          // '' => $comRel->field_produces_annual_report[LANGUAGE_NONE][0]['value'],
          // '' => $comRel->field_newsletter[LANGUAGE_NONE][0]['value'],
          // '' => $comRel->field_state_of_black_report[LANGUAGE_NONE][0]['value'],
          // '' => $comRel->field_maintains_website[LANGUAGE_NONE][0]['value'],
          // '' => $comRel->field_affiliate_website_address[LANGUAGE_NONE][0]['value'],
          // '' => $comRel->field_monthly_website_hits[LANGUAGE_NONE][0]['value'],
          // '' => $comRel->field_is_website_linked_to_nul[LANGUAGE_NONE][0]['value'],
          // '' => $comRel->field_produces_tv_or_cable_show[LANGUAGE_NONE][0]['value'],
          // '' => $comRel->field_produces_a_radio_show[LANGUAGE_NONE][0]['value'],
          // '' => $comRel->field_has_ad_marketing_campaign[LANGUAGE_NONE][0]['value'],
          // '' => $comRel->field_method_of_ad_marketing[LANGUAGE_NONE][0]['value'],
          // '' => $comRel->field_marketing_kit_or_pamphlet[LANGUAGE_NONE][0]['value'],

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

      $mainParams = array();
      $checkForIndiv = array(
        'contact_type' => "Individual",
        'sequential' => 1,
        'first_name' => $census->field_president_ceo_first_name[LANGUAGE_NONE][0]['value'],
        'last_name' => $census->field_president_ceo_last_name[LANGUAGE_NONE][0]['value'],
        'custom_1014' => "Affiliate CEO",
        // 'email' => $census->field_email_address[LANGUAGE_NONE][0]['value'],
      );
      $mainParams = nul_census_custom_assemble_params($mainParams, $checkForIndiv);
      $civiContact = nul_census_custom_civi_api('Contact', 'get', $mainParams);
      $civiContactId = 0;
      // TODO add fields to $checkForIndiv to update on the indiv contact record
      // IF only one contact found update it
      if ($civiContact['count'] == 1) {
        $civiContactId = $civiContact['values'][0]['id'];
      }
      // IF no contact found create one
      if ($civiContact['count'] == 0) {
        watchdog('nul_census_custom', 'No matching contact found for Affiliate CEO', array('%matchingAffiliates' => $civiAffiliate['values']), WATCHDOG_ERROR, $link = NULL);
        drupal_set_message('No matching CEO contact found for this Urban League Affiliate found please', 'error');
      }
      if ($civiContact['count'] > 1) {
        watchdog('nul_census_custom', 'More than one matching Contact found please de dupe', array('%matchingAffiliates' => $civiAffiliate['values']), WATCHDOG_ERROR, $link = NULL);
        drupal_set_message('More than one matching contact found for this Urban League Affiliate found please de dupe', 'error');
      }

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
    watchdog('nul_census_custom', 'No Affiliate Selected so we cannot sync to Civi', WATCHDOG_ERROR, $link = NULL);

  }
}
