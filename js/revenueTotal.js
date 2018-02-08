(function ($) {
  // THIS JS handles tables with total fields that need to be calcualted on the revenue page
  //TODO add revenue field names
  var fieldsToTotal = [];
  var totalField = '';
  $(document).ready(function () {
    // Disable total fields so user cannot enter content
    $(totalField).attr('readonly', true);
    $(totalField).addClass('readonly');

    // Function to calculate fields
    var calculateTotal = function (fieldsToTotal, totalField) {
      // set total to 0
      var total = parseInt(0);
      $(fieldsToTotal).each(function (i, field) {
        if (parseInt($(field).val())) {
          total = parseInt(total) + parseInt($(field).val());
        }
      });

      $('input#' + totalField).val(total);
    };

    // calculate total on page load
    calculateTotal(fieldsToTotal, totalField);

    // on key up recalculate total
    $(fieldsToTotal).each(function (index, fieldName) {
      $('input[id*="' + fieldName + '"').keyup(calculateTotal(fieldsToTotal, totalField));
    });
  });
}(jQuery));
