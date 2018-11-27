# Fields that autopopulate when you start a new census

## Census (Contact Information)

### Finding the Matching Affiliate Organization
based on the id in the url

### Finding the Matching Affiliate CEO
Defaults to the current user but one can change it using the CEO field I think this should be permissioned

### Finding the Matching Email, Phone, Addresses
Looks for a Email, Phone or Address for the contact of Location Type Main and picks the first one

### Fields

| Contact | Field Label in Drupal                  | Field Machine Name in Drupal     | Field in CiviCRM       | Location/Type |
|---------|----------------------------------------|----------------------------------|------------------------|---------------|
| Org     | Affiliate Select                       | field_affiliate_select           | Contact Id             | n/a           |
| Org     | Year Organization was Established      | field_date_established           | custom_1069            | n/a           |
| Org     | Address Line 1                         | field_address_line_1             | Street Address         | Main/Address  |
| Org     | Address line 2                         | field_address_line_2             | Supplemental Address 1 | Main/Address  |
| Org     | City                                   | field_city                       | City                   | Main/Address  |
| Org     | State/Province                         | field_state_province             | State/Province         | Main/Address  |
| Org     | Zip Code                               | field_zip_code                   | Postal Code            | Main/Address  |
| Org     | Telephone                              | field_telephone                  | Phone                  | Main/Phone    |
| Org     | Fax                                    | field_fax                        | Phone                  | Main/Fax      |
| CEO     | CEO                                    | field_ceo                        | Contact Id             | n/a           |
| CEO     | President/CEO First Name               | field_president_ceo_first_name   | First Name             | n/a           |
| CEO     | President/CEO Middle Name              | field_president_ceo_middle_name  | Middle Name            | n/a           |
| CEO     | President/CEO Last Name                | field_president_ceo_last_name    | Last Name              | n/a           |
| CEO     | Number of Years as CEO                 | field_number_of_years_as_ceo     | custom_1070            | n/a           |
| CEO     | Number of Years of Service in Movement | field_number_of_years_of_service | custom_1071            | n/a           |
| CEO     | Cellular Number                        | field_cellular_number            | Phone                  | Main/Mobile   |
| CEO     | Email Address                          | field_email_address              | Email                  | Main/Email    |
| CEO     | Photo                                  | field_photo                      | Contact Image          | n/a           |

## Revenue

### Fields

| Contact | Field Label in Drupal                  | Field Machine Name in Drupal     | Field in CiviCRM       | Location/Type |
|---------|----------------------------------------|----------------------------------|------------------------|---------------|
| Org     | How much investment earnings...?       | field_revenue_investment     	  |	custom_1098            | n/a           |
| Org     | Corporations                           | field_revenue_corporations   	  | custom_1099            | n/a           |
| Org     | Foundations       	                   | field_revenue_foundations      	| custom_1100            | n/a           |
| Org     | Individual Memberships      	         | field_revenue_individual_members | custom_1101            | n/a           |
| Org     | Special Events            	           | field_revenue_special_events     | custom_1102            | n/a           |
| Org     | United Way                             | field_revenue_united_way   	    | custom_1103            | n/a           |
| Org     | Federal                                | field_revenue_federal            | custom_1106            | n/a           |
| Org     | State/Local                            | field_revenue_state_local        | custom_1105            | n/a           |
| Org     | NUL               	                   | field_revenue_nul              	| custom_1107            | n/a           |
| Org     | Purpose of NUL Funding      	         | field_revenue_purpose_of_funding | custom_1108            | n/a           |
| Org     | Other                     	           | field_revenue_other              | custom_1109            | n/a           |
| Org     | Total revenue for your affiliate       | field_revenue_total_budget       | custom_1112            | n/a           |
| Org     | Does the affiliate have an endowment?  | field_revenue_has_endowment      | custom_1113            | n/a           |
| Org     | If so, what is the present amount?     | field_revenue_endowment_amount   | custom_1111            | n/a           |

## Community Relations

### Fields

| Contact | Field Label in Drupal                                              | Field Machine Name in Drupal     | Field in CiviCRM       | Location/Type |
|---------|--------------------------------------------------------------------|----------------------------------|------------------------|---------------|
| Org     | Does the affiliate produce an annual report?                       | field_produces_annual_report  	  |	custom_1084            | n/a           |
| Org     | Does the affiliate produce a monthly/quarterly newsletter?         | field_newsletter             	  | custom_1085            | n/a           |
| Org     | Does affiliate produce a "State of Black (Affiliate Name)" Report? | field_state_of_black_report     	| custom_1086            | n/a           |
| Org     | Does the affiliate maintain a website?                             | field_maintains_website          | custom_1087            | n/a           |
| Org     | If so, what is your affiliate's website address?                   | field_affiliate_website_address  | custom_1089            | n/a           |
| Org     | How many hits does your website receive monthly?                   | field_monthly_website_hits  	    | custom_1090            | n/a           |
| Org     | Is website linked to NUL website?                                  | field_is_website_linked_to_nul   | custom_1091            | n/a           |
| Org     | Does affiliate produce a regular TV or cable show?                 | field_produces_tv_or_cable_show  | custom_1092            | n/a           |
| Org     | Does affiliate produce a radio show?          	                   | field_produces_a_radio_show    	| custom_1093            | n/a           |
| Org     | Does affiliate have an advertising or marketing campaign?	         | field_has_ad_marketing_campaign  | custom_1094            | n/a           |
| Org     | What is the method of advertising or marketing?       	           | field_method_of_ad_marketing     | custom_1096            | n/a           |
| Org     | Does affiliate do a marketing kit and/or pamphlet?                 | field_marketing_kit_or_pamphlet  | custom_1095            | n/a           |

## TODO Service Areas
If on a new Service Area tab I think we should create one paragraph for each exisiting service area in civi for the affiliate then we would not be matching per say... we could create a new hidden field for the entity id so that we are matching exactly

### Matching Service Areas

### Fields

| Contact | Field Label in Drupal                  | Field Machine Name in Drupal     | Field in CiviCRM       | Location/Type |
|---------|----------------------------------------|----------------------------------|------------------------|---------------|
| Org     | Service Area City/County	             | field_service_area_city_county	  |	custom_1072            | n/a           |
| Org     | Service Area Population	               | field_service_area_population	  | custom_1073            | n/a           |
| Org     | Service Area White	                   | field_service_area_white	Decimal	| custom_1074            | n/a           |
| Org     | Service Area Hispanic/Latino	         | field_service_area_hispanic	    | custom_1075            | n/a           |
| Org     | Service Area Asian American	           | field_service_area_asian_am	    | custom_1076            | n/a           |
| Org     | Service Area Native American	         | field_service_area_native_am	    | custom_1077            | n/a           |
| Org     | Service Area African American	         | field_service_area_african_am	  | custom_1078            | n/a           |
| Org     | Service Area Other	                   | field_service_area_other	        | custom_1079            | n/a           |

## TODO Programs
When one clicks Add program they should be able to choose a program in civi to default to.

## TODO TEST USING 2019
# Questions:

1. Should we have a contact ref for address, email, Etc (allow user to pick from the exisiting ones in civi)
2. Assuming we are looking for phones, emails etc of type "Main"
3. What assumption should we make about who the CEO is if not basing it off of the logged in user or data entered into the first, last fields
