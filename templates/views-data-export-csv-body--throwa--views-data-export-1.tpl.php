<?php

// Array of labels keyed by field name
$labels = array(
  "field_total_expenditures" => "What was the total expenditure by your affiliate for expenses (include salaries, rent/mortgage, equipment, etc.)?",
  "field_a_salaries_wages" => "A. Salaries/Wages",
  "field_b_fringe_benefits" => "B. Fringe Benefits",
  "field_c_professional_fees" => "C. Professional/Contract/Consulting Fees",
  "field_d_travel" => "D. Travel",
  "field_e_postage_freight" => "E. Postage/Freight",
  "field_f_insurance" => "F. Insurance",
  "field_g_interest_payments" => "G. Interest Payments",
  "field_h_dues_subscription_regist" => "H. Dues/Subscription/Registration",
  "field_i_depreciation" => "I. Depreciation",
  "field_j_taxes_including_property" => "J. Taxes (including property taxes)",
  "field_k_utilities" => "K. Utilities (telephone, gas, electric)",
  "field_l_equipment_space_rental" => "L. Equipment/space rental",
  "field_m_goods_and_services" => "M. Goods and Services",
  "field_n_rent_mortgage_payments" => "N. Rent/mortgage payments",
  "field_o_other" => "O. Other",
  "field_number_properties_owned" => "How many properties does the affiliate own?",
  "field_number_properties_rented" => "How many properties does the affiliate rent?",
  "field_market_value_of_property" => "If the affiliate owns its facilities, what is the current market value of the property?",
  "field_capital_budget_amount" => "If so, how much?",
  "field_capital_budget" => "Does the affiliate have a capital budget?",
  "field_revenue_total_budget" => "What is the total budget of your affiliate?",
  "field_revenue_nul" => "NUL",
  "field_revenue_corporations" => "Corporations",
  "field_revenue_foundations" => "Foundations",
  "field_revenue_individual_members" => "Individual Memberships",
  "field_revenue_special_events" => "Special Events",
  "field_revenue_united_way" => "United Way",
  "field_revenue_federal" => "Federal",
  "field_revenue_state_local" => "State/Local",
  "field_revenue_investment" => "How much investment earnings (money market account, endowment)?",
  "field_revenue_ventures" => "Affiliate Social Entrepreneurship Ventures and/or Income Generating Activities",
  "field_revenue_has_endowment" => "Does the affiliate have an endowment?",
  "field_revenue_endowment_amount" => "If so, what is the present amount?",
  "field_program_ed_foster_total" => "If so, how many placements/recommendations for services do you make per year?",
  "field_program_entpr_new" => "Number of new businesses created",
  "field_program_entpr_sales" => "Total sales of businesses started by participants in entrepreneurship programs (i.e. Small Business Matters)",
  "field_program_housing_attended" => "How many participants attended or inquired about home ownership programs?",
  "field_program_housing_purchased" => "Number of program participants who purchased a home",
  "field_program_housing_avg_price" => "Average price of homes purchased",
  "field_program_work_welfare_salar" => "Average salary of welfare recipieints placed in jobs",
  "field_program_work_placed" => "Number of participants placed in jobs",
  "field_program_work_welfare" => "Number of welfare participants in federal/state funded programs",
  "field_program_work_welfare_place" => "Number of welfare program participants placed in jobs",
  "field_voter_societal_impact" => "Voter - Total Number Served or Registered (Male and Female)",
  "field_forums_societal_impact" => "Community Forums Societal Impact",
  "field_advocacy_societal_impact" => "Advocacy Efforts Societal Impact",
  "field_police_societal_impact" => "Police Brutality Societal Impact",
  "field_crja_societal_impact" => "Civil Rights & Justice Activities Societal Impact",
  "field_comm_org_societal_impact" => "Community Organization Societal Impact",
);

// Making Civic Enagement last instead of first
$civic = array_shift($themed_rows); $themed_rows[] = $civic;

foreach ($themed_rows as $count => $item_row):
  foreach ($item_row as $key => $value) {
    if ($key == 'type_1') {
      print $value . "\r\n";
    }
    elseif (!empty($value) && strlen($value) != 2) {
      print $separator . $labels[$key] . $separator . $value . "\r\n";
    }
  }
endforeach;
