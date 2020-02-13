Feature: Program Validation

SCENARIO: Tab with ability to add programs in view mode -> Selected Yes for our affiliate offers programs of this type and entered no programs (example)

WHEN on view of a tab for which a user has selected "Yes" for "Our Affiliate offers programs of this type"
AND no programs have been added
THEN a "Atleast one program must be entered to complete this tab" message is displayed
AND the "Mark Tab as complete" button is not displayed.

SCENARIO: Tab with ability to add programs in view mode -> Selected Yes for our affiliate offers programs of this type and entered programs (example)

WHEN on view of a tab for which a user has selected "Yes" for "Our Affiliate offers programs of this type"
AND programs have been added for this tab
THEN the "Mark Tab as complete" button is displayed.

SCENARIO: Tab with ability to add programs in view mode -> Selected NO for our affiliate offers programs of this type (example)

WHEN on view of a tab for which a user has selected "No" for "Our Affiliate offers programs of this type"
THEN the "Mark Tab as complete" button is displayed.

SCENARIO: Tab with programs in edit mode (example)
WHEN On edit of a tab with programs if one selects "Yes" for "Our Affiliate offers programs of this type *"
THEN they see a "You will be required to enter information about atleast one program to complete this tab on the next screen." message (like in attached Yes.png) if they select "No" for this field they do not see any message.

SCENARIO: Also test a tab without programs --- like service areas... which should just show the Mark as complete button.
