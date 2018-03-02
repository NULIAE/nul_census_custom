(function ($) {
  // AGH 11789 THIS JS handles tables with total fields
  // that need to be calcualted on the revenue page
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
      'input#edit-field-revenue-other-und-0-value',
    ];
    var totalField = 'input#edit-field-revenue-total-budget-und-0-value';

    // Disable total fields so user cannot enter content
    $(totalField).attr('readonly', true);
    $(totalField).addClass('readonly');

    // Disable other field which is a subtotal of the venture type fields
    $('input#edit-field-revenue-other-und-0-value').attr('readonly', true);
    $('input#edit-field-revenue-other-und-0-value').addClass('readonly');

    // Function to calculate fields
    var calculateTotalRevenue = function () {

      // add venture type fields together and save to other field
      var ventureTypeTotal = parseInt(0);
      $('input[id*="field-budget-venture-earned-und"]').each(function (i, field) {
        if (parseInt($(field).val())) {
          ventureTypeTotal = parseInt(ventureTypeTotal) + parseInt($(field).val());
        }
      });

      $('input#edit-field-revenue-other-und-0-value').val(ventureTypeTotal);

      // set total to 0
      var total = parseInt(0);

      // Add normal fields
      $(fieldsToTotal).each(function (i, field) {
        if (parseInt($(field).val())) {
          total = parseInt(total) + parseInt($(field).val());
        }
      });

      $(totalField).val(total);
    };

    // calculate total on page load
    calculateTotalRevenue(fieldsToTotal, totalField);

    // on key up for normal fields recalculate total
    $(fieldsToTotal).each(function (index, fieldName) {
      $(fieldName).keyup(calculateTotalRevenue);
    });

    // on key up for amount earned field in venture type paragraph recalculate total
    // (for existing paragraphs)
    $('input[id*="field-budget-venture-earned-und"]').keyup(calculateTotalRevenue);

    // when a new paragraph is added
    $(document).ajaxStop(function () {
      // on key up for amount earned field in venture type paragraph recalculate total
      $('input[id*="field-budget-venture-earned-und"]').keyup(calculateTotalRevenue);
    });
  });
}(jQuery));
