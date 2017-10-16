(function ($) {
  var tableFields = [
    'input#edit-field-voter-white-male-und-0-value',
    'input#edit-field-voter-white-female-und-0-value',
    'input#edit-field-voter-hispanic-male-und-0-value',
    'input#edit-field-voter-hispanic-female-und-0-value',
    'input#edit-field-voter-asian-amer-male-und-0-value',
    'input#edit-field-voter-asian-amer-female-und-0-value',
    'input#edit-field-voter-native-amer-male-und-0-value',
    'input#edit-field-voter-native-amer-female-und-0-value',
    'input#edit-field-voter-african-amer-male-und-0-value',
    'input#edit-field-voter-african-amer-female-und-0-value',
    'input#edit-field-voter-other-male-und-0-value',
    'input#edit-field-voter-other-female-und-0-value',
  ];
  $(document).ready(function () {
    // $('input#edit-field-voter-societal-impact-und-0-value').attr('disabled', true);
    var addFields = function () {
      var total = parseInt(0);
      $(tableFields).each(function (index, field) {
        if (parseInt($(field).val())) {
          total = parseInt(total) + parseInt($(field).val());
        }
      });

      $('input#edit-field-voter-societal-impact-und-0-value').val(total);
    };

    $(tableFields).each(function (index, field) {
      $(field).keyup(addFields);
    });
  });
}(jQuery));
