<?php

/**
 * Search Taxonomy by field on the taxonomy
 * @param  string $vocabMachineName  machine name of the vocabulary
 * @param  string $fieldName         name of field to search
 * @param  string $valueOfField      value to match on
 * @return object                    term object
 */
function nul_census_custom_searchTaxonomyByField($vocabMachineName, $fieldName, $valueOfField) {
  $term = NULL;
  $query = new EntityFieldQuery();
  $vocabulary = taxonomy_vocabulary_machine_name_load($vocabMachineName);
  $query->entityCondition('entity_type', 'taxonomy_term')
    ->propertyCondition('vid', $vocabulary->vid)
    ->fieldCondition($fieldName, 'value', $valueOfField);
  $terms = $query->execute();
  if (!empty($terms['taxonomy_term'])) {
    foreach ($terms['taxonomy_term'] as $key => $termDetails) {
      $term = taxonomy_term_load($termDetails->tid);
    }
  }
  return $term;
}

/**
 * Check permissions of user to edit or resubmit a tab or census based on status
 * @param  string $status status of tab
 * @param  string $contentType census or tab
 * @return array  can they edit or not, can they resubmit or not
 */
function nul_census_custom_check_status_permisions($status = NULL, $contentType, $nid = NULL) {
  $term = taxonomy_term_load($status);
  if (!empty($term->name)) {
    $status = $term->name;
  }
  global $user;
  $canResubmit = FALSE;
  $canEdit = FALSE;
  $canView = FALSE;
  $myAffiliate = FALSE;
  if (!empty($nid)) {
    // check that logged in user and node affiliate match
    $loggedInUserAffiliate = nul_census_custom_get_affiliate_from_user();
    $nodeAffiliateNode = nul_census_custom_get_affiliate_from_node($nid, $contentType);
    if ($loggedInUserAffiliate == $nodeAffiliateNode) {
      $myAffiliate = TRUE;
    }
  }

  // IF user is an administrator they can edit whatever always
  if (in_array('administrator', $user->roles) || user_access('edit all affiliate censuses')) {
    $canEdit = TRUE;
    $canResubmit = TRUE;
    $canView = TRUE;
  }
  // If user is not admin check permissions
  elseif ($myAffiliate && user_access('edit own affiliate censuses')) {
    $canView = TRUE;
    if ($contentType == 'tab') {
      switch ($status) {
        case 'Incomplete':
          $canEdit = TRUE;
          break;

        case 'Complete':
          if (user_access('edit census tabs with status submitted reviewed complete or review complete') || user_access('edit census with status complete')) {
            $canEdit = TRUE;
          }
          else {
            $canEdit = FALSE;
          }
          break;

        case 'Resubmit':
          if (user_access('edit census tabs with status resubmit')) {
            $canEdit = TRUE;
            $canResubmit = TRUE;
          }
          else {
            $canEdit = FALSE;
          }
          break;

        case 'Reviewed':
          if (user_access('edit census tabs with status submitted reviewed complete or review complete')) {
            $canEdit = TRUE;
          }
          else {
            $canEdit = FALSE;
          }
          break;

        case 'Reviewed Complete':
          if (user_access('edit census tabs with status submitted reviewed complete or review complete')) {
            $canEdit = TRUE;
          }
          else {
            $canEdit = FALSE;
          }
          break;

        case 'Submitted':
          if (user_access('edit census tabs with status submitted reviewed complete or review complete')) {
            $canEdit = TRUE;
          }
          else {
            $canEdit = FALSE;
          }
          break;

        default:
          $canEdit = FALSE;

          break;
      }
    }
    if ($contentType == 'census') {
      switch ($status) {
        case 'Incomplete':
          $canEdit = TRUE;
          break;

        case 'Complete':
          if (user_access('edit census with status complete')) {
            $canEdit = TRUE;
          }
          else {
            $canEdit = FALSE;
          }
          break;

        case 'Reviewed Complete':
          $canEdit = FALSE;
          break;

        case 'Submitted':
          if (user_access('edit census with status submitted')) {
            $canEdit = TRUE;
          }
          else {
            $canEdit = FALSE;
          }
          break;

        default:
          $canEdit = FALSE;

          break;

      }
    }
  }
  return array(
    'canEdit' => $canEdit,
    'canResubmit' => $canResubmit,
    'canView' => $canView,
  );
}

/**
 * checks status of related tabs
 * @param  int $nid parent census nid
 * @param  string $exceptThisType tab to skip
 * @param  string $statusName     status name to check for
 * @return array array with one complete array (tabs with the status) and one notComplete array (machine names of tabs that do not have the correct status)
 */
function nul_census_custom_check_tab_statuses($nid, $exceptThisType, $statusName) {
  $tabsReport = array(
    'complete' => array(),
    'notComplete' => array(),
  );
  // TODO check the status of the census before continuing to make sure we need to do all this
  $parentCensus = node_load($nid);
  // load census tab vocabulary
  $censusTabsVocabulary = taxonomy_vocabulary_machine_name_load('census_tabs');
  $censusTabs = taxonomy_get_tree($censusTabsVocabulary->vid);
  // foreach thru each term in census tab vocab
  foreach ($censusTabs as $key => $object) {
    if (!empty($object->tid)) {
      // load term
      $term = taxonomy_term_load($object->tid);
      // if there is a machine name content type for that term and it is not
      if (!empty($term->field_content_type_machine_name[LANGUAGE_NONE][0]['value'])) {
        if ($term->field_content_type_machine_name[LANGUAGE_NONE][0]['value'] !== $exceptThisType) {
          $completeTerm = taxonomy_get_term_by_name($statusName, 'census_tab_status');
          // search for node of that type AND parent census with NID passed to the function
          $query = new EntityFieldQuery();
          $query->entityCondition('entity_type', 'node')
            ->entityCondition('bundle', $term->field_content_type_machine_name[LANGUAGE_NONE][0]['value'])
            ->fieldCondition('field_parent_census', 'target_id', $nid)
            ->fieldCondition('field_tab_status', 'tid', key($completeTerm));
          $results = $query->execute();
          // If not found push machine name of not complete node
          if (empty($results['node'])) {
            $tabsReport['notComplete'][] = $term->field_content_type_machine_name[LANGUAGE_NONE][0]['value'];
          }
          // If found push data for complete tab to array
          if (!empty($results['node'])) {
            $tabsReport['complete'][key($results['node'])] = $results['node'][key($results['node'])];
          }
        }
      }
    }
  }
  return $tabsReport;
}

/**
 * update status fields on nodes
 * @param  array $tabsCompleted   array of tabs where key is the nid
 * @param  string $statusToChangeTo name of status to change to
 * @param string $censusPart is it a census or a tab
 */
function nul_census_custom_update_nodes_status_field($tabsCompleted, $statusToChangeTo, $censusPart) {
  if ($censusPart == 'tab') {
    $vocabMachineName = 'census_tab_status';
    $fieldName = 'field_tab_status';
  }
  if ($censusPart == 'census') {
    $vocabMachineName = 'status_type_for_census';
    $fieldName = 'field_census_status';
    // IF changing to reviewed complete update civi
    if ($statusToChangeTo == 'Reviewed Complete') {
      $censusId = key($tabsCompleted);
      nul_census_custom_update_civi($censusId);
    }
  }
  $submittedTabTerm = key(taxonomy_get_term_by_name($statusToChangeTo, $vocabMachineName));
  foreach ($tabsCompleted as $nid => $object) {
    $tab = node_load($nid);
    $tab->{$fieldName}[LANGUAGE_NONE][0]['tid'] = $submittedTabTerm;
    $tab = node_save($tab);
  }
}

function nul_census_custom_assemble_params($presetParams, $ifNotEmptyParams) {
  foreach ($ifNotEmptyParams as $key => $value) {
    if (!empty($value)) {
      $presetParams[$key] = $value;
    }
  }
  return $presetParams;
}
function nul_census_custom_update_civicontact_info($entity, $params) {
  $params['sequential'] = 1;
  $checkForExisting = nul_census_custom_civi_api($entity, 'get', $params);
  if ($checkForExisting['count'] >= 1 && !empty($checkForExisting['values'][0]['id'])) {
    $params['id'] = $checkForExisting['values'][0]['id'];
  }
  nul_census_custom_civi_api($entity, 'create', $params);
}

function nul_census_custom_update_civi($censusId) {
  $census = node_load($censusId);
  if (!empty($census->field_affiliate_select[LANGUAGE_NONE][0]['value'])) {
    $params = array(
      'contact_type' => "Organization",
      'sequential' => 1,
      // 'contact_sub_type' => "Urban_League_Affiliate",
      // 'custom_296' => $census->field_affiliate_select[LANGUAGE_NONE][0]['value'],
      'id' => 1,
    );
    $civiAffiliate = nul_census_custom_civi_api('Contact', 'get', $params);
    if (!empty($civiAffiliate['count']) && $civiAffiliate['count'] == 0 || $civiAffiliate == array()) {
      watchdog('nul_census_custom', 'No matching Urban League Affiliate found please make sure there is a valid org to connect to', WATCHDOG_NOTICE, $link = NULL);
      drupal_set_message('No matching Urban League Affiliate found please make sure there is a valid org to connect to', 'error');
    }
    if (!empty($civiAffiliate['count']) && $civiAffiliate['count'] > 1) {
      watchdog('nul_census_custom', 'More than one matching Urban League Affiliate found please de dupe', array('%matchingAffiliates' => $civiAffiliate['values']), WATCHDOG_ERROR, $link = NULL);
      drupal_set_message('More than one matching Urban League Affiliate found please de dupe', 'error');
    }
    if (!empty($civiAffiliate['count']) && $civiAffiliate['count'] == 1 && !empty($civiAffiliate['values'][0]['id'])) {
      // Find Community Relations Tab
      $comRel = nul_census_custom_load_tab('community_relations', $censusId);
      $comRel = node_load(key($comRel['node']));
      // Find Service Areas Tab
      $serviceAreas = nul_census_custom_load_tab('service_areas', $censusId);
      $serviceAreas = node_load(key($serviceAreas['node']));

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
          // TODO create custom field to hold the "Year Organization was Established" data
          // '' => $census->field_date_established[LANGUAGE_NONE][0]['value'],
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
      $orgAddressParams = array(
        'sequential' => 1,
        'contact_id' => $civiAffiliate['values'][0]['id'],
        'location_type_id' => "Main",
        'street_address' => $census->field_address_line_1[LANGUAGE_NONE][0]['value'],
        'supplemental_address_1' => $census->field_address_line_2[LANGUAGE_NONE][0]['value'],
        'city' => $census->field_city[LANGUAGE_NONE][0]['value'],
        'state_province_id' => $census->field_state_province[LANGUAGE_NONE][0]['value'],
        // 'postal_code' => $census->field_zip_code[LANGUAGE_NONE][0]['value'],
      );
      $orgAddress = nul_census_custom_assemble_params(array(), $orgAddressParams);
      nul_census_custom_update_civicontact_info('Address', $orgAddress);

      // Update Org Telephone
      if (!empty($census->field_telephone[LANGUAGE_NONE][0]['value'])) {
        nul_census_custom_update_civicontact_info('Phone', array(
          'contact_id' => $civiAffiliate['values'][0]['id'],
          'phone' => $census->field_telephone[LANGUAGE_NONE][0]['value'],
          'location_type_id' => "Main",
          'phone_type_id' => "Phone",
        ));
      }

      // Update Org Fax
      if (!empty($census->field_fax[LANGUAGE_NONE][0]['value'])) {
        nul_census_custom_update_civicontact_info('Phone', array(
          'contact_id' => $civiAffiliate['values'][0]['id'],
          'phone' => $census->field_fax[LANGUAGE_NONE][0]['value'],
          'location_type_id' => "Main",
          'phone_type_id' => "Fax",
        ));
      }

      $mainParams = array();
      $checkForIndiv = array(
        'contact_type' => "Individual",
        'sequential' => 1,
        'first_name' => $census->field_president_ceo_first_name[LANGUAGE_NONE][0]['value'],
        'last_name' => $census->field_president_ceo_last_name[LANGUAGE_NONE][0]['value'],
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
        $createIndivParams = $mainParams;
        $civiContactCreate = nul_census_custom_civi_api('Contact', 'create', $createIndivParams);
        $civiContactId = $civiContactCreate['id'];
      }
      if ($civiContact['count'] > 1) {
        // TODO figure out how to find best contact if more than one match
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

          // TODO create fields in civi to store this data
          // $census->field_number_of_years_as_ceo[LANGUAGE_NONE][0]['value'],
          // $census->field_number_of_years_of_service[LANGUAGE_NONE][0]['value'],
        );
        $civiContactUpdate = nul_census_custom_civi_api('Contact', 'create', $fieldsToUpdateOnContact);

        // Update Cell Phone
        if (!empty($census->field_cellular_number[LANGUAGE_NONE][0]['value'])) {
          $civiContactCell = nul_census_custom_update_civicontact_info('Phone', array(
            'contact_id' => $civiContactId,
            'phone' => $census->field_cellular_number[LANGUAGE_NONE][0]['value'],
            'location_type_id' => "Main",
            'phone_type_id' => "Mobile",
          ));
        }
        // Update Email
        if (!empty($census->field_email_address[LANGUAGE_NONE][0]['value'])) {
          $civiContactEmail = nul_census_custom_update_civicontact_info('Email', array(
            'contact_id' => $civiContactId,
            'email' => $census->field_email_address[LANGUAGE_NONE][0]['value'],
            'location_type_id' => "Main",
          ));
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

/**
 * [nul_census_custom_load_tab description]
 * @param  string $tabType  name of tab content type
 * @param  int $censusId    id of census
 * @return int              id of tab of this type for this census
 */
function nul_census_custom_load_tab($tabType, $censusId) {
  $query = new EntityFieldQuery();
  $results = $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', $tabType)
    ->fieldCondition('field_parent_census', 'target_id', $censusId)
    ->propertyCondition('status', NODE_PUBLISHED)
    ->execute();
  return $results;
}

function nul_census_custom_hide_and_disable_fields(&$form, $fieldsToHideAndDisable) {
  if (!user_access('can edit magic census fields')) {
    drupal_add_js(drupal_get_path('module', 'nul_census_custom') . '/js/hideFieldLabel.js');
    foreach ($fieldsToHideAndDisable as $key => $value) {
      if (!empty($form[$value])) {
        // TODO when disabling fields the drupal way content types with paragraphs do not save appropriately so using js workaround
        // $form[$value]['#disabled'] = TRUE;
        // $form[$value]['#type'] = 'hidden';
        $form[$value]['#attributes']['class'][] = 'hide';
      }
    }
  }
}

/**
 * adds button to change status of tab or node
 * @param  string $statusOfTab   status before clicking button
 * @param  string $statusTabMustBe  status to change to if button clicked
 * @param  string $permissionUserMustHave permission needed to see button
 * @param  string $censusPart    stage the census is in options tab = on tab with status submitted, census with status of submitted with all tabs with the status of reviewed, resubmit = tab with status of resubmit
 * @param  int $tabNid         Node id of the tab the user is on
 * @param  int $parentCensusId   Node id of the parent census
 * @param  array $argumentsAllowed  array of statuses that the user can update to ex: Submitted, Resubmit, Reviewed, ReviewComplete
 */
function nul_census_custom_button_to_update_status($statusOfTab, $statusTabMustBe, $permissionUserMustHave, $censusPart, $tabNid, $parentCensusId, $argumentsAllowed) {
  // IF on tab with status submitted and user has proper permissions add mark reviewed and resubmit buttons
  // TODO create tabs in menu hook instead of in js
  if (!empty($statusOfTab) && $statusOfTab == $statusTabMustBe && user_access($permissionUserMustHave)) {
    drupal_add_js(array('nul_census_custom' => array('censuspart' => $censusPart)), array('type' => 'setting'));
    drupal_add_js(array('nul_census_custom' => array('nid' => $tabNid)), array('type' => 'setting'));
    drupal_add_js(drupal_get_path('module', 'nul_census_custom') . '/js/addButtons.js');
    if (arg(0) == 'node' && arg(1) > 0 && in_array(arg(2), $argumentsAllowed)) {
      $tabsCompleted = array($tabNid => 'does not matter');
      nul_census_custom_update_nodes_status_field($tabsCompleted, arg(2), 'tab');
      drupal_set_message('Tab status updated', 'notice');
      if (!empty($parentCensusId)) {
        drupal_goto("node/" . $parentCensusId);
      }
      else {
        drupal_goto("node/" . arg(1));
      }
    }
  }
}

/**
 * get affiliate from node
 * @param  int $nid node id
 * @param string $censusPart tab or census
 * @return int $affiliate civicrm contact id of affiliate associated with this node
 */
function nul_census_custom_get_affiliate_from_node($nid, $censusPart) {
  $affiliate = NULL;
  if ($censusPart == 'tab') {
    $tab = node_load($nid);
    $nid = $tab->field_parent_census[LANGUAGE_NONE][0]['target_id'];
    $parentCensus = node_load($nid);
  }
  if ($censusPart == 'census') {
    $parentCensus = node_load($nid);
  }
  if (!empty($parentCensus->field_affiliate_select[LANGUAGE_NONE][0]['value'])) {
    $affiliate = $parentCensus->field_affiliate_select[LANGUAGE_NONE][0]['value'];
  }
  return $affiliate;
}

/**
 * Make a CiviCRM API call
 * @param  string $entity entity to target
 * @param  string $action get, create etc.
 * @param  array $params array of params to send
 * @return array $results civicrm api results
 */
function nul_census_custom_civi_api($entity, $action, $params) {
  $results = array();
  if (!civicrm_initialize()) {
    return NULL;
  }
  try {
    $results = civicrm_api3($entity, $action, $params);
  }
  catch (CiviCRM_API3_Exception $e) {
    $error = $e->getMessage();
    CRM_Core_Error::debug_log_message(ts('API Error %1', array(
      'domain' => 'nul_census_custom',
      1 => $error,
    )));
  }
  return $results;
}

/**
 * lookup the affiliate on the civicrm contact
 * @param  id $drupalUserId drupal user id
 * @return int    Civicrm Contact ID of the affiliate the user has on their contact record
 */
function nul_census_custom_get_affiliate_from_user($drupalUserId = NULL) {
  if ($drupalUserId == NULL) {
    global $user;
    $drupalUserId = $user->uid;
  }
  $affiliateField = "contact_id.custom_" . variable_get('affiliatefield');
  $affiliate = NULL;
  $params = array(
    'return' => array($affiliateField, "contact_id.id"),
    'uf_id' => $drupalUserId,
  );
  $civiContact = nul_census_custom_civi_api('UFMatch', 'getsingle', $params);
  if (!empty($civiContact[$affiliateField])) {
    $affiliate = $civiContact[$affiliateField];
  }
  return $affiliate;
}

/**
 * Check if user has permission to create a census for that affiliate
 * @param  int $affiliateId id of affiliate
 * @return boolean             true or false, can edit or can't
 */
function nul_census_custom_census_creation_permissions($affiliateId, $year) {
  $canCreate = array();
  // If no affiliate ID and user does not have permission to create census for any affiliate block them
  if (empty($affiliateId) && !user_access('can create census for any affiliate')) {
    $canCreate['any'] = FALSE;
    drupal_goto('/');
  }
  // If can only create censuses for their affiliate check that the id passed from the url is for an affiliate the user has access to
  if (user_access('can create census for own affiliate') && !user_access('can create census for any affiliate') && !user_access('can edit magic census fields')) {
    $canCreate['any'] = FALSE;
    // get Affiliate of user (on civi Contact record) no matter what direct to censuses of that affiliate
    $loggedInUserAffiliate = nul_census_custom_get_affiliate_from_user();
    // If user does not have an affiliate direct to home page
    if (empty($loggedInUserAffiliate)) {
      drupal_goto('/');
    }
    // IF user has an affiliate set it
    else {
      $canCreate['affiliateId'] = $loggedInUserAffiliate;
    }
  }
  // If an admin user can create
  if (user_access('can create census for any affiliate') || user_access('can edit magic census fields')) {
    $canCreate['affiliateId'] = $affiliateId;
    $canCreate['any'] = TRUE;
  }
  // CHECK for existing Census
  $query = new EntityFieldQuery();
  $results = $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'census')
    ->fieldCondition('field_affiliate_select', 'value', $canCreate['affiliateId'])
    ->fieldCondition('field_year', 'value', $year)
    ->execute();
  // If you find one go to it
  if (!empty($results['node'])) {
    $canCreate['existingCesus'] = key($results['node']);
    drupal_goto('/node/' . key($results['node']));
  }
  return $canCreate;
}

function nul_census_custom_check_all_fields_populated(&$node) {
  // TODO look up paragraphs fields and skip them
  $unfilledFields = array();
  foreach ($node as $property => $value) {
    if (substr($property, 0, 6) == 'field_') {
      if (empty($node->$property)) {
        $unfilledFields[] = $property;
      }
    }
  }
  return $unfilledFields;
}

/**
 * [nul_census_custom_get_emails_to_notify description]
 * @param  [type] $node [description]
 * @return [type]       [description]
 */
function nul_census_custom_get_emails_to_notify($node) {
  $toAndFrom = array(
    'to' => '',
    'from' => '',
  );
  // emails to notify
  $emailFields = array(
    'field_survey_email_address',
    'field_email_address',
  );
  $from = variable_get('emailnotification');
  $to[] = $from;
  foreach ($emailFields as $key => $field) {
    if (!empty($node->{$field}[LANGUAGE_NONE][0]['value'])) {
      $to[] = $node->{$field}[LANGUAGE_NONE][0]['value'];
    }
  }
  $to = implode(', ', $to);
  if (!empty($from)) {
    $toAndFrom['from'] = $from;
  }
  if (!empty($to)) {
    $toAndFrom['to'] = $to;
  }
  return $toAndFrom;
}

function nul_census_custom_get_status_of_programs_parent_details_tab($detailTabType, $parentCensus) {
  $programDetailsTab = NULL;
  // TODO look up program details
  $query = new EntityFieldQuery();
  $results = $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', $detailTabType)
    ->fieldCondition('field_parent_census', 'target_id', $parentCensus)
    ->execute();
  if (!empty($results['node']) && !empty(key($results['node']))) {
    $programDetailsTab = node_load(key($results['node']));
  }
  return $programDetailsTab;
}

function nul_census_custom_redirect_to_program_details_tab($form, &$form_state) {
  $nodeToRedirectTo = '';
  if (!empty($form['field_parent_census'][LANGUAGE_NONE]['#value'])) {
    $nodeToRedirectTo = $form['field_parent_census'][LANGUAGE_NONE]['#value'];
    $contentType = taxonomy_term_load($form['field_program_area'][LANGUAGE_NONE]['#value'])->field_content_type[LANGUAGE_NONE][0]['value'];
    $parentProgramTab = nul_census_custom_get_status_of_programs_parent_details_tab($contentType, $form['field_parent_census'][LANGUAGE_NONE]['#value']);
    if (!empty($parentProgramTab->nid)) {
      $nodeToRedirectTo = $parentProgramTab->nid . "/" . $form['field_parent_census'][LANGUAGE_NONE]['#value'];
    }
  }
  $form_state['redirect'] = "/node/$nodeToRedirectTo";
}

/**
 * identify which part of the census you are on ... census main node, tab node or a program
 * @param  string $nodeType node content type
 * @return string part of the census the node is ('census', 'tab', 'programs' or 'none')
 */
function nul_census_custom_get_census_part($nodeType) {
  $censusPart = 'none';
  if ($nodeType == 'census' || $nodeType == 'programs') {
    $censusPart = $nodeType;
  }
  else {
    $tabTermDetails = nul_census_custom_searchTaxonomyByField('census_tabs', 'field_content_type_machine_name', $nodeType);
    if (!empty($tabTermDetails)) {
      $censusPart = 'tab';
    }
  }
  return $censusPart;
}

function nul_census_custom_remove_commas($form_state) {
  // has number fields in paragraphs so def need to load
  if (!empty($form_state['node']->type) && in_array($form_state['node']->type, array('service_areas', 'programs', 'revenue', 'program_entrepreneurship'))) {
    drupal_add_js(array('nul_census_custom' => array('removecommas' => array())), array('type' => 'setting'));
    drupal_add_js(drupal_get_path('module', 'nul_census_custom') . '/js/removeComma.js');
  }
  $numberFields = array();
  if (!empty($form_state['field'])) {
    foreach ($form_state['field'] as $field => $fieldDets) {
      if (
        !empty($fieldDets[LANGUAGE_NONE]['field']['field_name']) &&
        !empty($fieldDets[LANGUAGE_NONE]['field']['type']) &&
        in_array($fieldDets[LANGUAGE_NONE]['field']['type'], array('number_decimal', 'number_integer'))
      ) {
        $numberFields[] = $fieldDets[LANGUAGE_NONE]['field']['field_name'];
      }
    }
  }
  if (!empty($numberFields)) {
    drupal_add_js(array('nul_census_custom' => array('removecommas' => $numberFields)), array('type' => 'setting'));
    drupal_add_js(drupal_get_path('module', 'nul_census_custom') . '/js/removeComma.js');
  }
  // return $numberFields;
}

function nul_census_custom_check_for_required_url_parts($part) {
  if (arg(0) == 'node' &&
  arg(1) == 'add' &&
  !empty(arg(2)) &&
  !user_access('can create census for any affiliate')) {
    // if not proper stuff sent
    if ((in_array($part, array('census', 'tab'))  && empty(arg(3))) || ($part == 'programs' && empty(arg(4)))) {
      drupal_set_message('No parent census id sent, user can only create programs for censuses of their affiliate');
      drupal_goto('/');
    }
  }
}

function nul_census_custom_export_to_csv($form) {
  // IF affiliate set add it to query
  if (!empty($form['field_affiliate_select_value']['#value'])) {
    $asClause = "" . $form['field_affiliate_select_value']['#value'];
    if ($form['field_affiliate_select_value']['#value'] == 'All') {
      $asClause = "";
    }
    else {
      $asClause = " AND affiliate.field_affiliate_select_value = " . $form['field_affiliate_select_value']['#value'];
    }
  }
  // IF year set add it to query
  if (!empty($form['field_year_value']['#value'])) {
    $yearClause = "" . $form['field_year_value']['#value'];
    if ($form['field_year_value']['#value'] == 'All') {
      $yearClause = "";
    }
    else {
      $yearClause = " AND ye.field_year_value = " . $form['field_year_value']['#value'];
    }
  }
  // if workforce development report
  if ($form['#id'] == 'views-exposed-form-workforce-development-report-page') {
    // SQL Query for Workforce
    $query = "(SELECT
    parentCensus.title as 'Title',
    ye.field_year_value as 'Year',
    pwc.field_program_work_counseling_value as '# clients who received counseling',
    pwp.field_program_work_participants_value as 'Number of participants in employment/workforce development programs (exclude welfare recipients)?',
    pwpl.field_program_work_placed_value as 'Number of participants placed in jobs',
    pws.field_program_work_salary_value as 'Annual salary (if applicable)',
    pwh.field_program_work_hourly_value as 'or Hourly wage rate',
    pww.field_program_work_welfare_value as 'Number of welfare participants in federal/state funded programs',
    pwwp.field_program_work_welfare_place_value as 'Number of welfare program participants placed in jobs',
    pwws.field_program_work_welfare_salar_value as 'Annual welfare salary (if applicable)',
    pwwh.field_program_work_welfare_hour_value as 'or Hourly wage rate (welfare)'
    FROM node as workforce
    LEFT JOIN field_data_field_parent_census parent
    ON workforce.nid = parent.entity_id
    LEFT JOIN node parentCensus
    ON parentCensus.nid = parent.field_parent_census_target_id
    LEFT JOIN field_data_field_year ye
    ON parentCensus.nid = ye.entity_id
    LEFT JOIN field_data_field_affiliate_select affiliate
    ON parentCensus.nid = affiliate.entity_id
    LEFT JOIN field_data_field_program_work_counseling pwc
    ON pwc.entity_id = workforce.nid
    LEFT JOIN field_data_field_program_work_participants pwp
    ON pwp.entity_id = workforce.nid
    LEFT JOIN field_data_field_program_work_placed pwpl
    ON pwpl.entity_id = workforce.nid
    LEFT JOIN field_data_field_program_work_salary pws
    ON pws.entity_id = workforce.nid
    LEFT JOIN field_data_field_program_work_hourly pwh
    ON pwh.entity_id = workforce.nid
    LEFT JOIN field_data_field_program_work_welfare pww
    ON pww.entity_id = workforce.nid
    LEFT JOIN field_data_field_program_work_welfare_place pwwp
    ON pwwp.entity_id = workforce.nid
    LEFT JOIN field_data_field_program_work_welfare_salar pwws
    ON pwws.entity_id = workforce.nid
    LEFT JOIN field_data_field_program_work_welfare_hour pwwh
    ON pwwh.entity_id = workforce.nid
    WHERE workforce.type = 'program_workforce'"
    . $yearClause . $asClause .
    ") UNION
    (SELECT
    '' as 'Title',
    '' as 'Year',
    SUM(pwc.field_program_work_counseling_value) as '# clients who received counseling',
    SUM(pwp.field_program_work_participants_value) as 'Number of participants in employment/workforce development programs (exclude welfare recipients)?',
    SUM(pwpl.field_program_work_placed_value) as 'Number of participants placed in jobs',
    SUM(pws.field_program_work_salary_value) as 'Annual salary (if applicable)',
    SUM(pwh.field_program_work_hourly_value) as 'or Hourly wage rate',
    SUM(pww.field_program_work_welfare_value) as 'Number of welfare participants in federal/state funded programs',
    SUM(pwwp.field_program_work_welfare_place_value) as 'Number of welfare program participants placed in jobs',
    SUM(pwws.field_program_work_welfare_salar_value) as 'Annual welfare salary (if applicable)',
    SUM(pwwh.field_program_work_welfare_hour_value) as 'or Hourly wage rate (welfare)'
    FROM node as workforce
    LEFT JOIN field_data_field_parent_census parent
    ON workforce.nid = parent.entity_id
    LEFT JOIN node parentCensus
    ON parentCensus.nid = parent.field_parent_census_target_id
    LEFT JOIN field_data_field_year ye
    ON parentCensus.nid = ye.entity_id
    LEFT JOIN field_data_field_affiliate_select affiliate
    ON parentCensus.nid = affiliate.entity_id
    LEFT JOIN field_data_field_program_work_counseling pwc
    ON pwc.entity_id = workforce.nid
    LEFT JOIN field_data_field_program_work_participants pwp
    ON pwp.entity_id = workforce.nid
    LEFT JOIN field_data_field_program_work_placed pwpl
    ON pwpl.entity_id = workforce.nid
    LEFT JOIN field_data_field_program_work_salary pws
    ON pws.entity_id = workforce.nid
    LEFT JOIN field_data_field_program_work_hourly pwh
    ON pwh.entity_id = workforce.nid
    LEFT JOIN field_data_field_program_work_welfare pww
    ON pww.entity_id = workforce.nid
    LEFT JOIN field_data_field_program_work_welfare_place pwwp
    ON pwwp.entity_id = workforce.nid
    LEFT JOIN field_data_field_program_work_welfare_salar pwws
    ON pwws.entity_id = workforce.nid
    LEFT JOIN field_data_field_program_work_welfare_hour pwwh
    ON pwwh.entity_id = workforce.nid
    WHERE workforce.type = 'program_workforce'
    " . $yearClause . $asClause . ")";

    $field_names = array(
      // '0' => 'Workforce Tab NID',
      // '1' => 'Parent Census NID',
      '2' => 'Title',
      '3' => 'Year',
      '4' => '# clients who received counseling',
      '5' => 'Number of participants in employment/workforce development programs (exclude welfare recipients)?',
      '6' => 'Number of participants placed in jobs',
      '7' => 'Annual salary (if applicable)',
      '8' => 'or Hourly wage rate',
      '9' => 'Number of welfare participants in federal/state funded programs',
      '10' => 'Number of welfare program participants placed in jobs',
      '11' => 'Annual welfare salary (if applicable)',
      '12' => 'or Hourly wage rate (welfare)',
    );

    $csvTitle = "workforce_report_";
  }
  $result = db_query($query);
  $result = $result->fetchAll();
  $csv_file = $csvTitle . date('Ymd') . ".csv";
  drupal_add_http_header('Content-Type', 'text/csv');
  header("Content-Disposition: attachment; filename=\"$csv_file\"");

  $fp = fopen('php://output', 'w');
  $first = TRUE;

  foreach ($result as $line) {
    //Gets the line so we can flip it and get the column names
    $column_names = get_object_vars($line);
    if ($first) {
      fputcsv($fp, $field_names);
      $first = FALSE;
      // Only happens once
    }
    // Puts the actual content
    fputcsv($fp, $column_names);
  }
  fclose($fp);
  drupal_exit();
}


function nul_census_custom_redirect_to_csv() {
  // for census_summary instead of rewriting view just reformated data export
  drupal_goto('/census-summary/export');
}
