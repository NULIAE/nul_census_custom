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
function nul_census_custom_check_status_permisions($status, $contentType) {
  global $user;
  $canResubmit = FALSE;
  // IF user is an administrator they can edit whatever always
  if (in_array('administrator', $user->roles)) {
    $canEdit = TRUE;
    $canResubmit = TRUE;
  }
  // If user is not admin check permissions
  if (!in_array('administrator', $user->roles)) {
    if ($contentType == 'tab') {
      switch ($status) {
        case 'Incomplete':
          $canEdit = TRUE;
          break;

        case 'Complete':
          if (user_access('edit census tabs with status submitted reviewed complete or review complete')) {
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
  }
  $submittedTabTerm = key(taxonomy_get_term_by_name($statusToChangeTo, $vocabMachineName));
  foreach ($tabsCompleted as $nid => $object) {
    $tab = node_load($nid);
    $tab->{$fieldName}[LANGUAGE_NONE][0]['tid'] = $submittedTabTerm;
    $tab = node_save($tab);
  }
}

function nul_census_custom_hide_and_disable_fields(&$form, $fieldsToHideAndDisable) {
  if (!user_access('can edit magic census fields')) {
    foreach ($fieldsToHideAndDisable as $key => $value) {
      if (!empty($form[$value])) {
        $form[$value]['#disabled'] = TRUE;
        // $form[$value]['#type'] = 'hidden';
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
 * Get Affliate (or affiliates) that the logged in user has access to
 * @return int $id contactId of the affilaite in civi
 */
function nul_census_custom_get_affiliate() {
  global $user;
  // TODO using drupal user look up civi affiliate
  $id = 18;
  return $id;
}
