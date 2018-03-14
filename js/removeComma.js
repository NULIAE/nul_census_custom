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

    // Strip commas out of number fields in paragraphs
    var paragraphNumberFields = [

      // Service Areas
      'field-service-area-population-und',

      // Revenue tab
      'field-budget-venture-earned-und',
      'field-budget-venture-budgeted-und',

      // Program TAB
      'field-funding-amount-und',
      'field-program-service-served-und',
      'field-program-service-dollars-und',
      'field-program-service-hours-und',

      // Entrepreneurship
      'field-business-sales-und',
      'field-business-served-und',
    ];

    var checkParagraphNumberFields = function (paragraphNumberFields) {
      $(paragraphNumberFields).each(function (index, fieldName) {
        console.log($('input[id*=' + fieldName + ']'));
        $('input[id*=' + fieldName + ']').change(function () {
          $(this).val(
            $(this).val().replace(/\,/g, '')
          );
          $(this).trigger('keyup');
        });
      });
    };

    checkParagraphNumberFields(paragraphNumberFields);

    $(document).ajaxStop(function () {
      checkParagraphNumberFields(paragraphNumberFields);
    });
  });
}(jQuery));
