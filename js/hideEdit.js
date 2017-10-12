(function ($) {
  $(document).ready(function () {
    // hide edit link on view of content tab if do not have permission
    $('a[href*="edit"]').each(function () {
      $(this).hide();
    });
  });
}(jQuery));
