This Module:
-----------

Styles fields on the census to create grids to improve user experience.

## Fields Styled

### Example: "What is the racial/gender composition of your board?" Table

This is done using groups created by the [field group](https://www.drupal.org/project/field_group) drupal module) and markup fields created by the drupal module [markup](https://www.drupal.org/project/markup).

Each table is a group (of type div) with the class "nulTable"
Each row is a group with the label for the group being the first row.
Table headers are made using markup fields for the cells and the class "nulTableHeader"

Then a style sheet is added that styles the div's using the display properties: "table", "table-row" and "table-cell" to appear as a table and hides the labels. There is also code in here that creates a tooltip for each of the input fields for which the labels are hidden.

See screenshot below:

![Screenshot of board composition table](img/boardCompositionTable.png)


All field groups styled this way:

1. What is the racial/gender composition of your board?
2. Voter Registration fields
3. Emergency Relief Activities
4. Direct Contact, Public Contact and Indirect Contact Data (A, B and C)
5. Community Forum
6. Community Organization

## Notes on Config

Because this node type is so large we needed to up the "max_input_vars" value to make everything save correctly. When using censusfeature and this custom module users should do the same. see documentation [here](https://stackoverflow.com/questions/9973555/setting-max-input-vars-php-ini-directive-using-ini-set) and below:

To change the max input vars setting I added:

```

php_value max_input_vars 3000
php_value suhosin.get.max_vars 3000
php_value suhosin.post.max_vars 3000
php_value suhosin.request.max_vars 3000

```

To the .htaccess file

## Census in edit mode

When you visit: <url>/node/add/test-clean-census/<affiliateId>
Hides tab fields affiliate and year
Populates the year to be the current year
Populates title to be the affiliate id concatenated with the year

Related tab content type npdes are created on first view with status of incomplete.

## Census in view mode

When one views the census in view mode styles table

## Permissions
For Census Tabs editing:

| status            | edit census tabs with status submitted reviewed complete or review complete | edit census tabs with status resubmit |  
| Complete          | Yes                                                                         | No                                    |  
| Incomplete        | Yes                                                                         | Yes                                   |  
| Resubmit          | No                                                                          | Yes                                   |  
| Reviewed          | Yes                                                                         | No                                    |  
| Reviewed Complete | Yes                                                                         | No                                    |  
| Submitted         | Yes                                                                         | No                                    |  

## Total-ing

Totals for tables on the following tabs:

 + Employees and Board Members: Board members table totals by race and grand total
 + Edit Contact Data (Direct, Indirect & Public): Public and Direct contact tables, totals by gender
 + Civic Engagement: Tables for Voter Registration, Community Organizations, Community Forums, CRJA, Police Brutality, Advocacy


## Taxonomy Terms

This module has an install file that creates the taxonomy terms needed for the feature

## Views Configuration

For the Back to Parent Census button:

these blocks needs to be configured to go at the bottom of the content region for the theme that is being used for edit mode:

+ http://nuldemo.aghstrategies.net/admin/structure/views/view/back_to_census_button_for_new/edit

These block needs to be configured to go to the bottom of the conent region for the front end theme and backend theme

+ http://nuldemo.aghstrategies.net/admin/structure/views/view/back_to_census_button/edit/
+ View: Go to Census in Status Mode
