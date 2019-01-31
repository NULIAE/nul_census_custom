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

    case 'service_areas':

      // TODO create the node with status not started
      $node = node_load();

      // TODO get all the existing service areas using ApI something like below or a db query
      $serviceAreas = nul_census_custom_civi_api('Customvalue', 'get', array('entity_id' => $affiliateIdInCivi));

      // create a paragraph for each exisiting service area
      foreach ($serviceArea as $key => $value) {
        $paragraph = new ParagraphsItemEntity(array('field_name' => 'name_of_field_in_node', 'bundle' => 'name_of_paragraph_bundle'));
        $paragraph->is_new = TRUE;
        $paragraph->setHostEntity('node', $node);
        $paragraph->field1_name_in_paragraph_bundle[LANGUAGE_NONE][0]['value'] = 'Test Data';
        $paragraph->field2_name_in_paragraph_bundle[LANGUAGE_NONE][0]['value'] = 'Test Data';
        $paragraph->save();
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

function nul_census_custom_autopopulate_from_last_year_census_node() {

}

function nul_census_custom__last_years_census_node($affiliateId) {
  $lastYearsCensusNode = NULL;
  $thisYear = date("Y");
  $lastYear = $thisYear - 1;
  $query = new EntityFieldQuery();
  $results = $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'census')
    ->fieldCondition('field_affiliate_select', 'value', $affiliateId)
    ->fieldCondition('field_year', 'value', $lastYear)
    ->execute();
  if (!empty($results['node'])) {
    $lastYearsCensusNode = key($results['node']);
  }
  return $lastYearsCensusNode;
}
