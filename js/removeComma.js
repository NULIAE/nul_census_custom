(function ($) {
  $(document).ready(function () {
    // THIS JS strips commas out of number fields
    var fieldsToStrip = Drupal.settings.nul_census_custom.removecommas;
    $(fieldsToStrip).each(function (index, fieldName) {
      $('input[name*=' + fieldName + ']').change(function () {
        $(this).val(
          $(this).val().replace(/\,/g, '')
        );
        $(this).trigger('keyup');
      });
    });
  });
}(jQuery));
