(function ($) {
  $(document).ready(function () {
    // THIS JS strips commas out of number fields
    var fieldsToStrip = [
      '#edit-field-revenue-investment-und-0-value',
    ];
    $(fieldsToStrip).each(function (index, fieldName) {
      $(fieldName).change(function () {
        $(fieldName).val(
          $(fieldName).val().replace(',', '')
        );
        $(fieldName).trigger('keyup');
      });

    });

  });
}(jQuery));
