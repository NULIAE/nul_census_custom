(function ($) {
  // For some reason table on emergency_relief_activities needs extra js
  $(document).ready(function () {
    $('.group-era .field-group-format-wrapper').contents().unwrap();
  });
}(jQuery));
