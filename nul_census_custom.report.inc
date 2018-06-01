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
  concat('$', format(sum(te.field_total_expenditures_value), 2)) as 'What was the total expenditure by your affiliate for expenses (include salaries rent/mortgage equipment etc.)?',
  concat('$', format(sum(ea.field_a_salaries_wages_value), 2)) as 'A. Salaries/Wages',
  concat('$', format(sum(eb.field_b_fringe_benefits_value), 2)) as 'B. Fringe Benefits',
  concat('$', format(sum(ec.field_c_professional_fees_value), 2)) as 'C. Professional/Contract/Consulting Fees',
  concat('$', format(sum(ed.field_d_travel_value), 2)) as 'D. Travel',
  concat('$', format(sum(ee.field_e_postage_freight_value), 2)) as 'E. Postage/Freight',
  concat('$', format(sum(ef.field_f_insurance_value), 2)) as 'F. Insurance',
  concat('$', format(sum(eg.field_g_interest_payments_value), 2)) as 'G. Interest Payments',
  concat('$', format(sum(eh.field_h_dues_subscription_regist_value), 2)) as 'H. Dues/Subscription/Registration',
  concat('$', format(sum(ei.field_i_depreciation_value), 2)) as 'I. Depreciation',
  concat('$', format(sum(ej.field_j_taxes_including_property_value), 2)) as 'J. Taxes (including property taxes)',
  concat('$', format(sum(ek.field_k_utilities_value), 2)) as 'K. Utilities (telephone gas electric)',
  concat('$', format(sum(el.field_l_equipment_space_rental_value), 2)) as 'L. Equipment/space rental',
  concat('$', format(sum(em.field_m_goods_and_services_value), 2)) as 'M. Goods and Services',
  concat('$', format(sum(en.field_n_rent_mortgage_payments_value), 2)) as 'N. Rent/mortgage payments',
  concat('$', format(sum(eo.field_o_other_value), 2)) as 'O. Other',
  SUM(eno.field_number_properties_owned_value) as 'How many properties does the affiliate own?',
  SUM(enr.field_number_properties_rented_value) as 'How many properties does the affiliate rent?',
  concat('$', format(sum(emv.field_market_value_of_property_value), 2)) as 'If the affiliate owns its facilities what is the current market value of the property?',
  concat('$', format(sum(ecba.field_capital_budget_amount_value), 2)) as 'If so how much?'
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

  $revQuery = "SELECT
  concat('$', format(SUM(rtb.field_revenue_total_budget_value), 2)) as 'What is the total budget of your affiliate?',
  concat('$', format(SUM(rn.field_revenue_nul_value), 2)) as 'NUL',
  concat('$', format(SUM(rc.field_revenue_corporations_value), 2)) as 'Corporations',
  concat('$', format(SUM(rf.field_revenue_foundations_value), 2)) as 'Foundations',
  concat('$', format(SUM(rim.field_revenue_individual_members_value), 2)) as 'Individual Memberships',
  concat('$', format(SUM(rse.field_revenue_special_events_value), 2)) as 'Special Events',
  concat('$', format(SUM(ruw.field_revenue_united_way_value), 2)) as 'United Way',
  concat('$', format(SUM(rfed.field_revenue_federal_value), 2)) as 'Federal',
  concat('$', format(SUM(rsl.field_revenue_state_local_value), 2)) as 'State/Local',
  concat('$', format(SUM(ro.field_revenue_other_value), 2)) as 'Other',
  concat('$', format(SUM(ri.field_revenue_investment_value), 2)) as 'How much investment earnings (money market account, endowment)?',
  concat('$', format(SUM(rv.field_revenue_ventures_value), 2)) as 'Affiliate Social Entrepreneurship Ventures and/or Income Generating Activities',
  concat('$', format(SUM(rhe.field_revenue_has_endowment_value), 2)) as 'Does the affiliate have an endowment?',
  concat('$', format(SUM(rea.field_revenue_endowment_amount_value), 2)) as 'If so, what is the present amount?'
  FROM node as tab
  LEFT JOIN field_data_field_parent_census parent
  ON tab.nid = parent.entity_id
  LEFT JOIN node parentCensus
  ON parentCensus.nid = parent.field_parent_census_target_id
  LEFT JOIN field_data_field_year ye
  ON parentCensus.nid = ye.entity_id
  LEFT JOIN field_data_field_affiliate_select affiliate
  ON parentCensus.nid = affiliate.entity_id
  LEFT JOIN field_data_field_revenue_total_budget rtb
  ON rtb.entity_id = tab.nid
  LEFT JOIN field_data_field_revenue_nul rn
  ON rn.entity_id = tab.nid
  LEFT JOIN field_data_field_revenue_corporations rc
  ON rc.entity_id = tab.nid
  LEFT JOIN field_data_field_revenue_foundations rf
  ON rf.entity_id = tab.nid
  LEFT JOIN field_data_field_revenue_individual_members rim
  ON rim.entity_id = tab.nid
  LEFT JOIN field_data_field_revenue_special_events rse
  ON rse.entity_id = tab.nid
  LEFT JOIN field_data_field_revenue_united_way ruw
  ON ruw.entity_id = tab.nid
  LEFT JOIN field_data_field_revenue_federal rfed
  ON rfed.entity_id = tab.nid
  LEFT JOIN field_data_field_revenue_state_local rsl
  ON rsl.entity_id = tab.nid
  LEFT JOIN field_data_field_revenue_other ro
  ON ro.entity_id = tab.nid
  LEFT JOIN field_data_field_revenue_investment ri
  ON ri.entity_id = tab.nid
  LEFT JOIN field_data_field_revenue_ventures rv
  ON rv.entity_id = tab.nid
  LEFT JOIN field_data_field_revenue_has_endowment rhe
  ON rhe.entity_id = tab.nid
  LEFT JOIN field_data_field_revenue_endowment_amount rea
  ON rea.entity_id = tab.nid
  WHERE tab.type = 'revenue' $whereClauses";

  $educationQuery = "SELECT
  SUM(eft.field_program_ed_foster_total_value) as 'If so, how many placements/recommendations for services do you make per year?)'
  FROM node as tab
  LEFT JOIN field_data_field_parent_census parent
  ON tab.nid = parent.entity_id
  LEFT JOIN node parentCensus
  ON parentCensus.nid = parent.field_parent_census_target_id
  LEFT JOIN field_data_field_year ye
  ON parentCensus.nid = ye.entity_id
  LEFT JOIN field_data_field_affiliate_select affiliate
  ON parentCensus.nid = affiliate.entity_id
  LEFT JOIN field_data_field_program_ed_foster_total eft
  ON eft.entity_id = tab.nid
  WHERE tab.type = 'program_education' $whereClauses";

  $entrepQuery = "SELECT
  SUM(etp.field_program_entpr_total_partic_value) as 'Total Participants in Entrepreneurship Programs',
  SUM(env.field_program_entpr_new_value) as 'Number of new businesses created',
  concat('$', format(SUM(es.field_program_entpr_sales_value), 2)) as 'Total sales of businesses started by participants in entrepreneurship programs (i.e. Small Business Matters)'
  FROM node as tab
  LEFT JOIN field_data_field_parent_census parent
  ON tab.nid = parent.entity_id
  LEFT JOIN node parentCensus
  ON parentCensus.nid = parent.field_parent_census_target_id
  LEFT JOIN field_data_field_year ye
  ON parentCensus.nid = ye.entity_id
  LEFT JOIN field_data_field_affiliate_select affiliate
  ON parentCensus.nid = affiliate.entity_id
  LEFT JOIN field_data_field_program_entpr_new env
  ON env.entity_id = tab.nid
  LEFT JOIN field_data_field_program_entpr_sales es
  ON es.entity_id = tab.nid
  LEFT JOIN field_data_field_program_entpr_total_partic etp
  ON etp.entity_id = tab.nid
  WHERE tab.type = 'program_entrepreneurship' $whereClauses";

  $housingQuery = "SELECT
  SUM(htp.field_program_housing_total_part_value) as 'How many participants attended or inquired about home ownership programs?',
  SUM(hp.field_program_housing_purchased_value) as 'Number of program participants who purchased a home',
  concat('$', format(AVG(hap.field_program_housing_avg_price_value), 2)) as 'Average price of homes purchased',
  SUM(hk.field_program_housing_have_kids_value) as 'How many people needing assistance have children under the age of 18 years of age?',
  SUM(ha.field_program_housing_alternate_value) as 'How many people were turned to alternative housing after losing their house?'
  FROM node as tab
  LEFT JOIN field_data_field_parent_census parent
  ON tab.nid = parent.entity_id
  LEFT JOIN node parentCensus
  ON parentCensus.nid = parent.field_parent_census_target_id
  LEFT JOIN field_data_field_year ye
  ON parentCensus.nid = ye.entity_id
  LEFT JOIN field_data_field_affiliate_select affiliate
  ON parentCensus.nid = affiliate.entity_id
  LEFT JOIN field_data_field_program_housing_total_part htp
  ON htp.entity_id = tab.nid
  LEFT JOIN field_data_field_program_housing_purchased hp
  ON hp.entity_id = tab.nid
  LEFT JOIN field_data_field_program_housing_avg_price hap
  ON hap.entity_id = tab.nid
  LEFT JOIN field_data_field_program_housing_have_kids hk
  ON hk.entity_id = tab.nid
  LEFT JOIN field_data_field_program_housing_alternate ha
  ON ha.entity_id = tab.nid
  WHERE tab.type = 'program_housing' $whereClauses";

  $workforceQuery = "SELECT
  SUM(pwc.field_program_work_counseling_value) as 'Number of clients who received workforce development/job placement counseling from your affiliate last year?',
  SUM(pwp.field_program_work_participants_value) as 'Number of participants in employment/workforce development programs (exclude welfare recipients)?',
  SUM(pwpl.field_program_work_placed_value) as 'Number of participants placed in jobs',
  SUM(pww.field_program_work_welfare_value) as 'Number of welfare participants in federal/state funded programs',
  SUM(pwwp.field_program_work_welfare_place_value) as 'Number of welfare program participants placed in jobs',
  concat('$', format(SUM(pws.field_program_work_salary_value), 2)) as 'Annual salary (if applicable)',
  concat('$', format(SUM(pwws.field_program_work_welfare_salar_value), 2)) as 'Annual welfare salary (if applicable)',
  SUM(pwwpl.field_program_work_welfare_place_value) as 'Number of welfare program participants placed in jobs',
  SUM(pwtp.field_program_work_total_partici_value) as 'Total for number of participants in employment/workforce development programs and number of welfare participants in federal/stat'
  FROM node as tab
  LEFT JOIN field_data_field_parent_census parent
  ON tab.nid = parent.entity_id
  LEFT JOIN node parentCensus
  ON parentCensus.nid = parent.field_parent_census_target_id
  LEFT JOIN field_data_field_year ye
  ON parentCensus.nid = ye.entity_id
  LEFT JOIN field_data_field_affiliate_select affiliate
  ON parentCensus.nid = affiliate.entity_id
  LEFT JOIN field_data_field_program_work_counseling pwc
  ON pwc.entity_id = tab.nid
  LEFT JOIN field_data_field_program_work_participants pwp
  ON pwp.entity_id = tab.nid
  LEFT JOIN field_data_field_program_work_placed pwpl
  ON pwpl.entity_id = tab.nid
  LEFT JOIN field_data_field_program_work_welfare pww
  ON pww.entity_id = tab.nid
  LEFT JOIN field_data_field_program_work_welfare_place pwwp
  ON pwwp.entity_id = tab.nid
  LEFT JOIN field_data_field_program_work_salary pws
  ON pws.entity_id = tab.nid
  LEFT JOIN field_data_field_program_work_welfare_salar pwws
  ON pwws.entity_id = tab.nid
  LEFT JOIN field_data_field_program_work_welfare_place pwwpl
  ON pwwpl.entity_id = tab.nid
  LEFT JOIN field_data_field_program_work_total_partici pwtp
  ON pwtp.entity_id = tab.nid
  WHERE tab.type = 'program_workforce' $whereClauses";

  $civicQuery = "SELECT
  SUM(vsi.field_voter_societal_impact_value) as 'Voter - Total Number Served or Registered (Male and Female)',
  SUM(vct.field_comm_org_total_value) as 'Community Organization Societal Impact',
  SUM(fsi.field_forums_societal_impact_value) as 'Community Forums Societal Impact',
  SUM(csi.field_crja_societal_impact_value) as 'Civil Rights & Justice Activities Societal Impact',
  SUM(psi.field_police_societal_impact_value) as 'Police Brutality Societal Impact',
  SUM(asi.field_advocacy_societal_impact_value) as 'Advocacy Efforts Societal Impact'
  FROM node as tab
  LEFT JOIN field_data_field_parent_census parent
  ON tab.nid = parent.entity_id
  LEFT JOIN node parentCensus
  ON parentCensus.nid = parent.field_parent_census_target_id
  LEFT JOIN field_data_field_year ye
  ON parentCensus.nid = ye.entity_id
  LEFT JOIN field_data_field_affiliate_select affiliate
  ON parentCensus.nid = affiliate.entity_id
  LEFT JOIN field_data_field_voter_societal_impact vsi
  ON vsi.entity_id = tab.nid
  LEFT JOIN field_data_field_comm_org_total vct
  ON vct.entity_id = tab.nid
  LEFT JOIN field_data_field_forums_societal_impact fsi
  ON fsi.entity_id = tab.nid
  LEFT JOIN field_data_field_crja_societal_impact csi
  ON csi.entity_id = tab.nid
  LEFT JOIN field_data_field_police_societal_impact psi
  ON psi.entity_id = tab.nid
  LEFT JOIN field_data_field_advocacy_societal_impact asi
  ON asi.entity_id = tab.nid
  WHERE tab.type = 'civic_engagement' $whereClauses";

  $eraQuery = "SELECT
  SUM(ts.field_relief_total_served_value) as 'Total Served'
  FROM node as tab
  LEFT JOIN field_data_field_parent_census parent
  ON tab.nid = parent.entity_id
  LEFT JOIN node parentCensus
  ON parentCensus.nid = parent.field_parent_census_target_id
  LEFT JOIN field_data_field_year ye
  ON parentCensus.nid = ye.entity_id
  LEFT JOIN field_data_field_affiliate_select affiliate
  ON parentCensus.nid = affiliate.entity_id
  LEFT JOIN field_data_field_relief_total_served ts
  ON ts.entity_id = tab.nid
  WHERE tab.type = 'emergency_relief_activities' $whereClauses";

  $csvTitle = "census_summary_report_";
  $csv_file = $csvTitle . date('Ymd') . ".csv";
  drupal_add_http_header('Content-Type', 'text/csv');
  header("Content-Disposition: attachment; filename=\"$csv_file\"");

  $fp = fopen('php://output', 'w');
  // title row
  $affiliateTitle = '';
  if ($form['field_affiliate_select']['#value'] == 'All') {
    $affiliateTitle = $form['field_affiliate_select']['#value'];
  }
  else {
    $affiliateTitle = $form['field_affiliate_select']['#options'][$form['field_affiliate_select']['#value']];
  }
  fputcsv($fp, array("{$form['field_year']['#value']} {$affiliateTitle}", '', ''));

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
