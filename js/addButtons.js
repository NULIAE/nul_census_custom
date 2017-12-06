(function ($) {
  $(document).ready(function () {
    var href = window.location.pathname;
    if (Drupal.settings.nul_census_custom.censuspart == 'tab') {
      var buttonsToAdd = "<li><a href='"
       + href  +
       "/Reviewed'>Mark Reviewed</a></li><li><a href='"
       + href  + "/Resubmit'>Mark Resubmit</a></li>";
    }

    if (Drupal.settings.nul_census_custom.censuspart == 'census') {
      var buttonsToAdd = "<li><a href='"
       + href  +
       "/ReviewComplete'>Mark Census Review Complete</a></li";
    }

    // Insert buttons to mark as reviewed and resubmit
    $($('ul.tabs.primary')).prepend(buttonsToAdd);
  });
}(jQuery));
