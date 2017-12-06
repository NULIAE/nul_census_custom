(function ($) {
  $(document).ready(function () {
    var href = window.location.pathname;

    // Insert buttons to mark as reviewed and resubmit
    $($('ul.tabs.primary')).prepend(
      "<li><a href='" + href  + "/Reviewed'>Mark Reviewed</a></li><li><a href='" + href  + "/Resubmit'>Mark Resubmit</a></li>"
    );
  });
}(jQuery));
