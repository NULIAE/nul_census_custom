(function ($) {
  // This hides the program details fields unless or until
  // the user clicks "Do you offer Programs of this type?"
  $(document).ready(function () {
    hideFields();
    $('div#edit-field-do-you-offer-programs-of-t').append("<div class='note'>You will be required to enter information about programs</div>");


    $('input[type=radio][name^=field_do_you_offer_programs_of_t]').change(hideFields);
    function hideFields() {
      // console.log($('input[type=radio][name^=field_do_you_offer_programs_of_t]:checked').val());
      if ($('input[type=radio][name^=field_do_you_offer_programs_of_t]:checked').val() == 1) {
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
	$('.note').show();
        $('div.field-group-div').each(function (index, key) {
          $(key).show();
        });
      }

      if ($('input[type=radio][name^=field_do_you_offer_programs_of_t]:checked').val() == 0 ||
      typeof (
        $('input[type=radio][name^=field_do_you_offer_programs]:checked').val())  === 'undefined'
      ) {
        $('.note').hide();
        $('.node-form div.form-item').each(function (index, key) {
          // skips the title, tab status and parent census fields,
          // whether the user sees those fields should be depenedent on their permissions
          if (!$(key).hasClass('form-item-field-do-you-offer-programs-of-t-und') &&
          !$(key).hasClass('form-item-title') &&
          !$(key).hasClass('form-item-field-parent-census-und') &&
          !$(key).hasClass('form-item-field-tab-status-und')) {
            $(key).hide();

            // AGH #11664 If user switches to saying
            // they do not have programs of that type clear all the fields

            // if its radio treat special
            if ($(key).children(':radio').length > 0) {
              $(key).children('input[type="radio"]').attr('checked', false);

            // IF its not a radio just clear value
            } else {
              $(key).children('input').val('');
            }
          }
        });

        $('div.field-group-div').each(function (index, key) {
          $(key).hide();
          $(key).val('');
        });
      }
    };
  });
}(jQuery));
