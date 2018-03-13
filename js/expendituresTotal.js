(function ($) {
  // AGH #12124 THIS JS handles tables with total fields
  // that need to be calcualted on the expenditures page
  $(document).ready(function () {
    var fieldsToTotal = [
      'input#edit-field-a-salaries-wages-und-0-value',
      'input#edit-field-b-fringe-benefits-und-0-value',
      'input#edit-field-c-professional-fees-und-0-value',
      'input#edit-field-d-travel-und-0-value',
      'input#edit-field-e-postage-freight-und-0-value',
      'input#edit-field-f-insurance-und-0-value',
      'input#edit-field-g-interest-payments-und-0-value',
      'input#edit-field-h-dues-subscription-regist-und-0-value',
      'input#edit-field-i-depreciation-und-0-value',
      'input#edit-field-k-utilities-und-0-value',
      'input#edit-field-l-equipment-space-rental-und-0-value',
      'input#edit-field-m-goods-and-services-und-0-value',
      'input#edit-field-n-rent-mortgage-payments-und-0-value',
      'input#edit-field-o-other-und-0-value',
    ];
    var totalField = 'input#edit-field-total-expenditures-und-0-value';

    // Disable total fields so user cannot enter content
    $(totalField).attr('readonly', true);
    $(totalField).addClass('readonly');

    // Function to calculate fields
    var calculateTotalExpenditure = function () {

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
    calculateTotalExpenditure(fieldsToTotal, totalField);

    // on key up for normal fields recalculate total
    $(fieldsToTotal).each(function (index, fieldName) {
      $(fieldName).keyup(calculateTotalExpenditure);
    });
  });
}(jQuery));
