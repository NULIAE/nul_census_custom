This Module:
-----------

Styles fields on the census to create grids to improve user experience.

## Fields Styled

### Example: "What is the racial/gender composition of your board?" Table

This is done using groups created by the [field group](https://www.drupal.org/project/field_group) drupal module) and markup fields created by the drupal module [markup](https://www.drupal.org/project/markup).

All these fields are in the group "What is the racial/gender composition of your board?" which is given the class "nulTable" thru the [ui]()

Rows are made into groups with in the "What is the racial/gender composition of your board?" group with the css class "nulTableRow"

Then a style sheet is added that styles the div's using the display properties: "table", "table-row" and "table-cell" to appear as a table and hides the labels. There is also code in here that creates a tooltip for each of the input fields for which the labels are hidden.

See screenshot below:

![Screenshot of board composition table](img/boardCompositionTable.png)


All field groups styled this way:

1. What is the racial/gender composition of your board?
2. Voter Registration fields


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
