(function ($) {
  // AGH 11789 THIS JS handles tables with total fields that need to be calcualted on the revenue page
  $(document).ready(function () {
    var fieldsToTotal = [
      'input#edit-field-revenue-investment-und-0-value',
      'input#edit-field-revenue-corporations-und-0-value',
      'input#edit-field-revenue-foundations-und-0-value',
      'input#edit-field-revenue-individual-members-und-0-value',
      'input#edit-field-revenue-special-events-und-0-value',
      'input#edit-field-revenue-united-way-und-0-value',
      'input#edit-field-revenue-federal-und-0-value',
      'input#edit-field-revenue-state-local-und-0-value',
      'input#edit-field-revenue-nul-und-0-value',
    ];
    var totalField = 'input#edit-field-revenue-total-budget-und-0-value';

    // Disable total fields so user cannot enter content
    $(totalField).attr('readonly', true);
    $(totalField).addClass('readonly');

    // Function to calculate fields
    var calculateTotalRevenue = function () {

      // set total to 0
      var total = parseInt(0);
      $(fieldsToTotal).each(function (i, field) {
        if (parseInt($(field).val())) {
          total = parseInt(total) + parseInt($(field).val());
        }
      });

      $(totalField).val(total);
    };

    // calculate total on page load
    calculateTotalRevenue(fieldsToTotal, totalField);

    // on key up recalculate total
    $(fieldsToTotal).each(function (index, fieldName) {
      $(fieldName).keyup(calculateTotalRevenue);
    });
  });
}(jQuery));
