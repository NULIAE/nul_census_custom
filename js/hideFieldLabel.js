(function ($) {
  $(document).ready(function () {
    // hide labels of fields that have .hide class
    $('input.hide').each(function (index, value) {
      var fieldID = $(value).attr('id');
      $('label[for="' + fieldID + '"]').addClass('hide');
    });
  });
}(jQuery));
