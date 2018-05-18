<?php
/**
 * Compiles where clauses for custom csv exports
 * @param  int $year        year to filter on
 * @param  int $affiliate   affiliate id to filter on
 * @return string           where clause
 */
function nul_census_custom_compile_whereClause($year, $affiliate) {
  $asClause = "";
  $yearClause = "";
  if (!empty($affiliate)) {
    if ($affiliate == 'All') {
      $asClause = "";
    }
    else {
      $asClause = " AND affiliate.field_affiliate_select_value = " . $affiliate;
    }
  }
  // IF year set add it to query
  if (!empty($year)) {
    if ($year == 'All') {
      $yearClause = "";
    }
    else {
      $yearClause = " AND ye.field_year_value = " . $year;
    }
  }
  return $yearClause . $asClause;
}
/**
 * [WorkForce Development Report Export
 * @param  obj $form  form object
 */
function nul_census_custom_export_to_csv($form) {
  $whereClauses = nul_census_custom_compile_whereClause($form['field_year_value']['#value'], $form['field_affiliate_select_value']['#value']);
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
    . $whereClauses .
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
    " . $whereClauses . ")";

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

/**
 * Census Summary Report Custom Export
 * @param  obj $form  form object
 */
function nul_census_custom_summary_export_to_csv($form) {
  // Where clauses (year and affiliate)
  $whereClauses = nul_census_custom_compile_whereClause($form['field_year']['#value'], $form['field_affiliate_select']['#value']);

  // ExpendituresTab
  $expQuery = "SELECT
  SUM(te.field_total_expenditures_value) as 'What was the total expenditure by your affiliate for expenses (include salaries rent/mortgage equipment etc.)?',
  SUM(ea.field_a_salaries_wages_value)  as 'A. Salaries/Wages',
  SUM(eb.field_b_fringe_benefits_value) as 'B. Fringe Benefits',
  SUM(ec.field_c_professional_fees_value) as 'C. Professional/Contract/Consulting Fees',
  SUM(ed.field_d_travel_value) as 'D. Travel',
  SUM(ee.field_e_postage_freight_value) as 'E. Postage/Freight',
  SUM(ef.field_f_insurance_value) as 'F. Insurance',
  SUM(eg.field_g_interest_payments_value) as 'G. Interest Payments',
  SUM(eh.field_h_dues_subscription_regist_value) as 'H. Dues/Subscription/Registration',
  SUM(ei.field_i_depreciation_value) as 'I. Depreciation',
  SUM(ej.field_j_taxes_including_property_value) as 'J. Taxes (including property taxes)',
  SUM(ek.field_k_utilities_value) as 'K. Utilities (telephone gas electric)',
  SUM(el.field_l_equipment_space_rental_value)  as 'L. Equipment/space rental',
  SUM(em.field_m_goods_and_services_value) as 'M. Goods and Services',
  SUM(en.field_n_rent_mortgage_payments_value) as 'N. Rent/mortgage payments',
  SUM(eo.field_o_other_value) as 'O. Other',
  SUM(eno.field_number_properties_owned_value) as 'How many properties does the affiliate own?',
  SUM(enr.field_number_properties_rented_value) as 'How many properties does the affiliate rent?',
  SUM(emv.field_market_value_of_property_value) as 'If the affiliate owns its facilities what is the current market value of the property?',
  SUM(ecba.field_capital_budget_amount_value) as 'If so how much?'
  FROM node as tab
  LEFT JOIN field_data_field_parent_census parent
  ON tab.nid = parent.entity_id
  LEFT JOIN node parentCensus
  ON parentCensus.nid = parent.field_parent_census_target_id
  LEFT JOIN field_data_field_year ye
  ON parentCensus.nid = ye.entity_id
  LEFT JOIN field_data_field_affiliate_select affiliate
  ON parentCensus.nid = affiliate.entity_id
  LEFT JOIN field_data_field_total_expenditures te
  ON te.entity_id = tab.nid
  LEFT JOIN field_data_field_a_salaries_wages ea
  ON ea.entity_id = tab.nid
  LEFT JOIN field_data_field_b_fringe_benefits eb
  ON eb.entity_id = tab.nid
  LEFT JOIN field_data_field_c_professional_fees ec
  ON ec.entity_id = tab.nid
  LEFT JOIN field_data_field_d_travel ed
  ON ed.entity_id = tab.nid
  LEFT JOIN field_data_field_e_postage_freight ee
  ON ee.entity_id = tab.nid
  LEFT JOIN field_data_field_f_insurance ef
  ON ef.entity_id = tab.nid
  LEFT JOIN field_data_field_g_interest_payments eg
  ON eg.entity_id = tab.nid
  LEFT JOIN field_data_field_h_dues_subscription_regist eh
  ON eh.entity_id = tab.nid
  LEFT JOIN field_data_field_i_depreciation ei
  ON ei.entity_id = tab.nid
  LEFT JOIN field_data_field_j_taxes_including_property ej
  ON ej.entity_id = tab.nid
  LEFT JOIN field_data_field_k_utilities ek
  ON ek.entity_id = tab.nid
  LEFT JOIN field_data_field_l_equipment_space_rental el
  ON el.entity_id = tab.nid
  LEFT JOIN field_data_field_m_goods_and_services em
  ON em.entity_id = tab.nid
  LEFT JOIN field_data_field_n_rent_mortgage_payments en
  ON en.entity_id = tab.nid
  LEFT JOIN field_data_field_o_other eo
  ON eo.entity_id = tab.nid
  LEFT JOIN field_data_field_number_properties_owned eno
  ON eno.entity_id = tab.nid
  LEFT JOIN field_data_field_number_properties_rented enr
  ON enr.entity_id = tab.nid
  LEFT JOIN field_data_field_market_value_of_property emv
  ON emv.entity_id = tab.nid
  LEFT JOIN field_data_field_capital_budget_amount ecba
  ON ecba.entity_id = tab.nid
  WHERE tab.type = 'expenditures' $whereClauses";

  $revQuery = "";
  $educationQuery = "";
  $entrepQuery = "";
  $housingQuery = "";
  $workforceQuery = "";
  $civicQuery = "";
  $eraQuery = "";

  $csvTitle = "census_summary_report_";
  $csv_file = $csvTitle . date('Ymd') . ".csv";
  drupal_add_http_header('Content-Type', 'text/csv');
  header("Content-Disposition: attachment; filename=\"$csv_file\"");

  $fp = fopen('php://output', 'w');
  // Format Tabs
  nul_census_custom_tab_formatting_for_census($expQuery, 'Expenditures', $fp);
  nul_census_custom_tab_formatting_for_census($revQuery, 'Revenue', $fp);
  nul_census_custom_tab_formatting_for_census($educationQuery, 'Education Programs', $fp);
  nul_census_custom_tab_formatting_for_census($entrepQuery, 'Entrepreneurship and Business Development Programs', $fp);
  nul_census_custom_tab_formatting_for_census($housingQuery, 'Housing and Community Development Programs', $fp);
  nul_census_custom_tab_formatting_for_census($workforceQuery, 'Workforce Development Programs', $fp);
  nul_census_custom_tab_formatting_for_census($civicQuery, 'Civic Engagement', $fp);
  nul_census_custom_tab_formatting_for_census($eraQuery, 'Emergency Relief Activities', $fp);

  fclose($fp);
  drupal_exit();
}

function nul_census_custom_tab_formatting_for_census($query, $tabName, $fp) {
  $first = TRUE;
  $result = db_query($query);
  $result = $result->fetchAll();

  foreach ($result as $line) {
    //Gets the line so we can flip it and get the column names
    $column_names = get_object_vars($line);
    if ($first) {
      fputcsv($fp, array($tabName, '', ''));
      $first = FALSE;
      // Only happens once
    }
    foreach ($line as $fieldName => $value) {
      fputcsv($fp, array('', $fieldName, $value));
    }
  }
}

function nul_census_custom_form($form, &$form_state) {
  $year = date('Y');
  $fieldsToAdd = array(
    'field_year' => array(
      'default_value' => $year,
      'title' => 'Year',
    ),
    'field_affiliate_select' => array(
      'default_value' => 'All',
      'title' => 'Affiliate',
    ),
  );
  foreach ($fieldsToAdd as $fieldName => $fieldHardcodedDetails) {
    $fieldInfo = field_info_field($fieldName);
    if (empty($fieldInfo['settings']['allowed_values'][$fieldHardcodedDetails['default_value']])) {
      $fieldInfo['settings']['allowed_values'][$fieldHardcodedDetails['default_value']] = $fieldHardcodedDetails['default_value'];
    }
    $form[$fieldName] = array(
      '#title' => $fieldHardcodedDetails['title'],
      '#type' => 'select',
      '#default_value' => $fieldHardcodedDetails['default_value'],
      '#options' => $fieldInfo['settings']['allowed_values'],
    );
  }
  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  $form['export_button'] = array(
    '#type' => 'submit',
    '#value' => t('Export to csv'),
  );
  $form['export_button']['#submit'][] = 'nul_census_custom_summary_export_to_csv';

  return $form;
}

function nul_census_custom_form_submit($form, &$form_state) {
  $link = "/census-summary-report/{$form['field_year']['#value']}/";
  if ($form['field_affiliate_select']['#value'] !== 'All') {
    $link = $link . "{$form['field_affiliate_select']['#value']}";
  }
  drupal_goto($link);
}
