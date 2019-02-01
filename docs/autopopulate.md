# Fields that autopopulate when you start a new census

## Census (Contact Information)

### Finding the Matching Affiliate Organization
based on the Affiliate Select id in the url

### Finding the Matching Affiliate CEO
Looks in civi for a contact that meets the following criteria:
Has the value "Affiliate CEO" for the custom civi field "Additional groups"
Has the Affiliate Select Id for the custom field Affiliate

### Finding the Matching Email, Phone, Addresses
Looks for a Email, Phone or Address for the contact of Location Type Main and picks the first one

### Fields

| Contact | Field Label in Drupal                  | Field Machine Name in Drupal     | Field in CiviCRM       | Location/Type |
|---------|----------------------------------------|----------------------------------|------------------------|---------------|
| Org     | Affiliate Select                       | field_affiliate_select           | custom_296             | n/a           |
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

## Service Areas
These are paragraphs so there could be more than one, They are populated from last years census

## Employees and Board members
Populated from last years census
