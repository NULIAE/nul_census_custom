(function ($) {
  $(document).ready(function () {
    // TODO this may not be necessary now with new permissions
    // hide edit link on view of content tab if do not have permission
    $('a[href*="edit"]').each(function () {
      $(this).hide();
    });
  });
}(jQuery));
