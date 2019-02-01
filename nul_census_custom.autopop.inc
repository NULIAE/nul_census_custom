<?php

/**
 * Autopopulate the tab based on the content in civi
 * @param  string $tab               name of content type
 * @param  int $affiliateIdInCivi    affiliate organization contact id in Civi
 * @param  int $affiliateCEO         affiliate ceo contact id in Civi (only for census)
 */
function nul_census_custom_autopopulate_from_civi(&$form, $tab, $affiliateIdInCivi, $affiliateCEO = NULL) {
  $fieldMapping = nul_census_custom_fieldmapping($tab);
  switch ($tab) {
    case 'census':
     $orgInCivi = nul_census_custom_civi_api('Contact', 'getsingle', array('id' => $affiliateIdInCivi, 'return' => array_values($fieldMapping['org'])));

      if (!empty($affiliateCEO)) {
        $CEOInCivi = nul_census_custom_civi_api('Contact', 'getsingle', array('id' => $affiliateCEO, 'return' => array_values($fieldMapping['CEO'])));
foreach ($fieldMapping['CEO'] as $fieldInDrupal => $fieldInCivi) {
          if (!empty($CEOInCivi[$fieldInCivi])) {
            // TODO set value for CEO entity reference field
            if ($fieldInDrupal == 'field_civicrm_ceo_contact_id') {
              // $form[$fieldInDrupal][LANGUAGE_NONE][0]['contact_id']['#default_value'] = $CEOInCivi[$fieldInCivi];
            }
            $form[$fieldInDrupal][LANGUAGE_NONE][0]['value']['#default_value'] = $CEOInCivi[$fieldInCivi];
          }
        }
        foreach ($fieldMapping['org'] as $fieldInDrupal => $fieldInCivi) {
          if (!empty($orgInCivi[$fieldInCivi])) {
            $form[$fieldInDrupal][LANGUAGE_NONE][0]['value']['#default_value'] = $orgInCivi[$fieldInCivi];
          }
        }
        foreach ($fieldMapping['contact'] as $fieldInDrupal => $civiMatchDets) {
          $params = array(
            'location_type_id' => 'Main',
            'sequential' => 1,
          );
          if ($civiMatchDets['contact'] == 'CEO') {
            $params['contact_id'] = $affiliateCEO;
          }
          if ($civiMatchDets['contact'] == 'org') {
            $params['contact_id'] = $affiliateIdInCivi;
          }
          if (!empty($civiMatchDets['phone_type_id'])) {
            $params['phone_type_id'] = $civiMatchDets['phone_type_id'];
          }
          $contactInfo = nul_census_custom_civi_api($civiMatchDets['entity'], 'get', $params);
          if (!empty($contactInfo['values'][0][$civiMatchDets['entity']])) {
            $form[$fieldInDrupal][LANGUAGE_NONE][0]['value']['#default_value'] = $contactInfo['values'][0][$civiMatchDets['entity']];
          }
        }
        $orgAddress = nul_census_custom_civi_api('Address', 'get', array(
          'contact_id' => $affiliateIdInCivi,
          'location_type_id' => 'Main',
          'sequential' => 1,
        ));
        if (!empty($orgAddress['values'][0]['location_type_id'])) {
          foreach ($fieldMapping['address'] as $drupalField => $civiField) {
            if (!empty($orgAddress['values'][0][$civiField])) {
              if ($civiField == 'state_province_id') {
                  $states = nul_census_custom_civi_api('Address', 'getoptions', array('field' => "state_province_id"));
                  if (!empty($states['values'][$orgAddress['values'][0]['state_province_id']])) {
                    $form[$drupalField][LANGUAGE_NONE]['#default_value'][] = $form[$drupalField][LANGUAGE_NONE]['#options'][$states['values'][$orgAddress['values'][0]['state_province_id']]];
                  }
              }

              $form[$drupalField][LANGUAGE_NONE][0]['value']['#default_value'] = $orgAddress['values'][0][$civiField];
            }
          }
        }
      }
      break;

    case 'community-relations':
      $orgInCivi = nul_census_custom_civi_api('Contact', 'getsingle', array('id' => $affiliateIdInCivi, 'return' => array_keys($fieldMapping)));
      foreach ($fieldMapping as $civiField => $drupalField) {
        if (!empty($form[$drupalField][LANGUAGE_NONE]['#options'][$orgInCivi[$civiField]])) {
          $form[$drupalField][LANGUAGE_NONE]['#default_value'] = $orgInCivi[$civiField];
        } else {
          $form[$drupalField][LANGUAGE_NONE][0]['value']['#default_value'] = $orgInCivi[$civiField];
        }
      }
      break;

    default:
      break;
  }

}

/**
 * populate Service Areas paragraphs from last year
 * @param  int $affiliateId affiliate select id
 * @param  string $contentType name of content type ('service_areas')
 * @param  object $node        node object
 */
function nul_census_custom_autopopulate_service_areas_paragraphs($affiliateId, $contentType, $node) {
  // Load last years Service Area tab
  $lastYearsTab = nul_census_custom_last_years_node($affiliateId, $contentType);

  $serviceAreaParagraphFields = array(
    'field_service_area_city_county',
    'field_service_area_population',
    'field_service_area_white',
    'field_service_area_hispanic',
    'field_service_area_asian_am',
    'field_service_area_native_am',
    'field_service_area_african_am',
    'field_service_area_other',
  );
  // Foreach thru last years service area paragraphs
  foreach ($lastYearsTab->field_service_areas[LANGUAGE_NONE] as $key => $values) {
    // Load last years paragraph
    $lastYearsParagraph = entity_load('paragraphs_item', array($values['value']));

    // create new paragraph
    $paragraph = new ParagraphsItemEntity(array('field_name' => 'field_service_areas', 'bundle' => 'service_area'));
    $paragraph->is_new = TRUE;
    $paragraph->setHostEntity('node', $node);
    foreach ($serviceAreaParagraphFields as $key => $field) {
      $paragraph->{$field}[LANGUAGE_NONE][0]['value'] = $lastYearsParagraph[$values['value']]->{$field}[LANGUAGE_NONE][0]['value'];
    }
    $paragraph->save();
  }
}

/**
 * auto populate basic fields from last years drupal census right now only education tab
 * @param  int $affiliateId    affiliate select id
 * @param  array $form        form from drupal
 */
function nul_census_custom_autopopulate_from_last_year_census_node($affiliateId, &$form) {
  $lastYearsTab = nul_census_custom_last_years_node($affiliateId, $form['type']['#value']);
  $fieldsToIgnore = array(
    'field_tab_status',
    'field_parent_census',
  );
  foreach ($lastYearsTab as $fieldName => $fieldDetails) {
    // If fieldName starts with field,
    // And the field is not field_tab_status or field_parent_census
    // And the field is on this years census
    if (substr($fieldName, 0, 5) == 'field' && !in_array($fieldName, $fieldsToIgnore) && !empty($form[$fieldName])) {
      // If field is radio or select
      if (!empty($form[$fieldName][LANGUAGE_NONE]['#options'])) {
        $form[$fieldName][LANGUAGE_NONE]['#default_value'] = $fieldDetails[LANGUAGE_NONE][0]['value'];
      }
      // If field is Text
      else {
        $form[$fieldName][LANGUAGE_NONE][0]['value']['#default_value'] = $fieldDetails[LANGUAGE_NONE][0]['value'];
      }
    }
  }
}

/**
 * get last years census from affilaite id
 * @param  int $affiliateId affiliate custom field id
 * @return string content type
 */
function nul_census_custom_last_years_node($affiliateId, $contentType) {
  $lastYearsCensusNode = NULL;
  $thisYear = date("Y");
  $lastYear = $thisYear - 1;
  $query = new EntityFieldQuery();
  $censusNode = $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'census')
    ->fieldCondition('field_affiliate_select', 'value', $affiliateId)
    ->fieldCondition('field_year', 'value', $lastYear)
    ->execute();
  if (!empty($censusNode['node'])) {
    $lastYearsCensusNode = key($censusNode['node']);
    $query = new EntityFieldQuery();
    $results = $query->entityCondition('entity_type', 'node')
      ->entityCondition('bundle', $contentType)
      ->fieldCondition('field_parent_census', 'target_id', $lastYearsCensusNode)
      ->execute();
    if (!empty($results['node']) && !empty(key($results['node']))) {
      $lastYearsTab = node_load(key($results['node']));
    }
  }
  return $lastYearsTab;
}
