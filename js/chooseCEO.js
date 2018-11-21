(function ($) {
  $(document).ready(function () {

  $('input#edit-field-civicrm-ceo-contact-id-und-0-contact-id').change(function() {
    var matches = $('input#edit-field-civicrm-ceo-contact-id-und-0-contact-id').val().match(/\[(.*?)\]/);
    if (matches) {
      var submatch = matches[1].replace('cid:', '');
        console.log(submatch);
      }
      console.log($('input#edit-field-civicrm-ceo-contact-id-und-0-contact-id').val());
   });
 });
}(jQuery));
