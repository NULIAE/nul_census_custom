# Dedupe rules when syncing drupal census data to civi

## Finding the Affiliate

Search for:

A Organization contact
Contact_sub_type = "Urban_League_Affiliate"
Has that affiliate id as the custom field for "affiliate"

THEN

IF one found: use that contact and proceed
IF none found: throw error
IF more than one found:

Check if any are members of the ACL Group "Urban League Affiliates" if so use that contact

IF not throw error.

## Finding the CEO
Search For:

Contact Type = Individual
First Name and Last Name entered
Has the value "Affiliate CEO" for the custom civi field "Additional groups"
Has the Affiliate Select Id for the custom field Affiliate

THEN

IF one found: use that contact and proceed
IF none found: throw an error
IF more than one found: throw error

## Finding the phone number/addresses for the Affiliate and CEO
Search For:

Contact Id of affliate or Ceo
Location Type ID (ex: Main)
*If relevant Phone type_id (ex: Fax)

THEN

IF one found: update based on data entered
IF none found: create a new record
IF more than one found: create a new record
