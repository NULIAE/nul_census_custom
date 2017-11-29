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

## Census in edit mode

When you visit: <url>/node/add/census/<affiliateId>

Custom code:

Hides the fields affiliate and year and populates them
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

### For the Back to Parent Census button:

these blocks needs to be configured to go at the bottom of the content region for the theme that is being used for edit mode:

+ http://nuldemo.aghstrategies.net/admin/structure/views/view/back_to_census_button_for_new/edit

These block needs to be configured to go to the bottom of the content region for the front end theme and backend theme

+ http://nuldemo.aghstrategies.net/admin/structure/views/view/back_to_census_button/edit/
+ View: Go to Census in Status Mode

### For the Program Census Tabs:

From the following 6 tabs one can create a program that is related to that tab:

+ [Education and Youth Development](http://nuldemo.aghstrategies.net/admin/structure/views/view/education_details_view/edit/block)
+ [Entrepreneurship and Business Development](http://nuldemo.aghstrategies.net/admin/structure/views/view/program_entrepreneurship/edit)
+ [Health and Quality of Life](http://nuldemo.aghstrategies.net/admin/structure/views/view/program_health/edit)
+ [Housing and Community Development](http://nuldemo.aghstrategies.net/admin/structure/views/view/program_housing/edit)
+ [Workforce Development](http://nuldemo.aghstrategies.net/admin/structure/views/view/program_workforce/edit)
+ [Other](http://nuldemo.aghstrategies.net/admin/structure/views/view/program_other/edit)

To do so one needs to configure the taxonomy term [program_areas](http://nuldrupal.localhost/taxonomy/term/651/edit?destination=admin/structure/taxonomy/%20program_areas) where the Name is the name of the program type and teh content type is the machine name of the content type one can create that type of program from.

And these views:

1. To add a program: http://nuldrupal.localhost/admin/structure/views/view/button_to_add_new_program/edit

This creates a link: <a href=/node/add/programs/[field_parent_census-target_id]/[type]>Add a Program</a> where [field_parent_census-target_id] is the id of the parent census and [type] is the content type. There is custom code on the add program content page that looks for these two parameters and sets the parent census value and the program type using the program area taxonomy

2. List of programs: http://nuldrupal.localhost/admin/structure/views/view/programs_for_program_area/edit

This view shows the programs that have the same parent census as the program details tab AND have the related (thru the program areas taxonmy) program type.

# Send Notification Emails when a new census is Created

# Send Notification Emails when the census status changes to "completed" "approved" or "resubmit"
