(function ($) {
  // ensure users can only click "Service Areas" tab once
  $(document).ready(function () {
    $('div.tab a').on('click', function (e) {
      $('div.tab a').prop('disabled', true);
    });
  });
}(jQuery));
