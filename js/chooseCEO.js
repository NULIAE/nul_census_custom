(function ($) {
  $(document).ready(function () {
  // TODO do this on page load not just on change... or maybe that should happen in php and only admins should be able to change this field...

  // on change of ceo contact update the relevant ceo fields
  $('input#edit-field-civicrm-ceo-contact-id-und-0-contact-id').change(function() {
    // get the ceo id from the CEO field
    var ceoFieldValue = $('input#edit-field-civicrm-ceo-contact-id-und-0-contact-id').val().match(/\[(.*?)\]/);
      if (ceoFieldValue) {
        var ceoId = ceoFieldValue[1].replace('cid:', '');

        // Look up the CEO Selected using the API
        CRM.api3('Contact', 'getsingle', {
          "sequential": 1,
          "id": ceoId
        }).done(function(result) {
          // TODO populate relevant fields
        });
      }

   });
 });
}(jQuery));
