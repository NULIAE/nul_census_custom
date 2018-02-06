(function ($) {
  // For some reason table on emergency_relief_activities needs extra js
  $(document).ready(function () {
    // options sorted by tid of program area set from php
    var sortedOptions = Drupal.settings.nul_census_custom.sorted;

    // if Program Area is set on page load get Services For that program area and save to variable
    if ($('#edit-field-program-area-und').val() > 0) {
      var programArea = $('#edit-field-program-area-und').val();
      var servicesForArea = sortedOptions[programArea];
    }

    // When a new service provided paragraph is added
    $(document).ajaxStop(function () {
      // foreach thru options and remove ones that arent for the program area
      if ($('select[name^="field_services_provided"]').not('[id*="weight"]').length > 0) {
        $('select[name^="field_services_provided"]').not('[id*="weight"]').each(function (p, v) {
          $(this).find('option').each(function (id, value) {
            if (!servicesForArea[id]) {
              $(value).remove();
            }
          });
        });
      }
    });
  });
}(jQuery));
