This Module:
-----------

Styles fields on the census to create grids to improve user experience.

## Fields Styled to be displayed as tables in View and Edit

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

## Census in edit mode

When you visit: <url>/node/add/census/<affiliateId>

Custom code: disables the affiliate, year and title fields and populates them as follows:

+ Affiliate to be the value passed as the 4th argument in the url (later this will be the civi contact id of the affiliate and be used for permissioning)
+ Year to be the current year
+ Populates the title to be the affiliate id concatenated with the year

## Permissions
For Census Tabs editing:

| status            |  status submitted reviewed complete or review complete | status resubmit |  
|-------------------|--------------------------------------------------------|-----------------|
| Complete          | Yes                                                    | No              |  
| Incomplete        | Yes                                                    | Yes             |  
| Resubmit          | No                                                     | Yes             |  
| Reviewed          | Yes                                                    | No              |  
| Reviewed Complete | Yes                                                    | No              |  
| Submitted         | Yes                                                    | No              |

## Total-ing

Totals for tables on the following tabs:

 + Employees and Board Members: Board members table totals by race and grand total
 + Edit Contact Data (Direct, Indirect & Public): Public and Direct contact tables, totals by gender
 + Civic Engagement: Tables for Voter Registration, Community Organizations, Community Forums, CRJA, Police Brutality, Advocacy


## Taxonomy Terms

This module has an install file that creates the taxonomy terms needed for the features in NULCENSUS_FEATURES (because features does not create list terms only the vocabulary) including:

+ Census Tab Status - These terms are the status's used for the census tabs
+ Status type for Census  - These terms are the status's used for the census
+ Census Tabs - This Taxonomy maps which content types are census tabs and uses it to create the menu
+ Program Areas - Each Program is a designated Program Area, this is a field on the program and determines which census tab the program should be listed under
+ Program Types - This is a field on the program that is limited by what program area the field program is designated as
+ Services Provided - This is a field on the program that is limited by what program area the field program is designated as

## Views Block Configuration:

### View: back to census button for new: Back to Census Button
This block is used to create a button to return to the Parent Census when in edit mode of a new census tab.

Configuration: This block should be added to the bottom of the content region of the theme being used for edit mode. And set title of the block to <none>

### View: Button to Add new Program

This creates a link: <a href=/node/add/programs/[field_parent_census-target_id]/[type]>Add a Program</a> where [field_parent_census-target_id] is the id of the parent census and [type] is the content type. There is custom code on the add program content page that looks for these two parameters and sets the parent census value and the program type using the program area taxonomy.

Configuration: This block should be configured to show at the bottom of the content region of the default theme. On all the program details content types (Education Program Details, Entrepreneurship and Business Development Program Details, Health and Quality of Life Program Details, Other Programs) and set title of block to <none>

### View: Programs for Program Area:

This view shows the programs that have the same parent census as the program details tab AND have the related (thru the program areas taxonomy) program type.

Configuration: Configure this block to show on all Program Details Tabs

### View: Census Tab Menu:

This block produces the menu of census tabs. It uses the Taxonomy "Census Tabs" and checks for a node of the content type in the "content type machine name" field and the parent census. If it finds one it links to it with an additional url parameter of the parent census which is used by the "programs for programs area" block to display the programs. IF it does not find one it links to node/add/{content_type}/{parent_census} which will take you to add a node of that content type with the parent census field set. There are two blocks one for when you are on the Census it self and one for the census tabs.

Configuration:

+ Census Tabs (on Census) - This should be configured to only show up on content type Census
+ Census Tabs (on Tab) -- This should be configured to only show up on the census content types.

## Sends Notification Emails when a new census is Created

## Sends Notification Emails when the census status changes to "completed" "approved" or "resubmit"
