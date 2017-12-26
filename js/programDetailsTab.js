(function ($) {
  // This hides the program details fields unless or until
  // the user clicks "Do you offer Programs of this type?"
  $(document).ready(function () {
    hideFields();
    $('input#edit-field-do-you-offer-programs-of-t-und').change(hideFields);

    function hideFields() {
      if ($('input#edit-field-do-you-offer-programs-of-t-und').attr('checked') == true) {
        $('.node-form div.form-item').each(function (index, key) {
          // skips the title, tab status and parent census fields,
          // whether the user sees those fields should be depenedent on their permissions
          if (!$(key).hasClass('form-item-field-do-you-offer-programs-of-t-und') &&
          !$(key).hasClass('form-item-title') &&
          !$(key).hasClass('form-item-field-parent-census-und') &&
          !$(key).hasClass('form-item-field-tab-status-und')) {
            $(key).show();
          }
        });
      }

      if ($('input#edit-field-do-you-offer-programs-of-t-und').attr('checked') == false) {
        $('.node-form div.form-item').each(function (index, key) {
          // skips the title, tab status and parent census fields,
          // whether the user sees those fields should be depenedent on their permissions
          if (!$(key).hasClass('form-item-field-do-you-offer-programs-of-t-und') &&
          !$(key).hasClass('form-item-title') &&
          !$(key).hasClass('form-item-field-parent-census-und') &&
          !$(key).hasClass('form-item-field-tab-status-und')) {
            $(key).hide();
          }
        });
      }
    };
  });
}(jQuery));
