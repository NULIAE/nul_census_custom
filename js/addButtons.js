(function ($) {
  // TODO JS to add butons to update the status of tabs
  //  and the census... move to hook menu when possible
  $(document).ready(function () {
    var nid = Drupal.settings.nul_census_custom.nid;
    var href = '/node/' + nid;
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

    if (Drupal.settings.nul_census_custom.censuspart == 'resubmit') {
      var buttonsToAdd = "<li><a href='"
       + href  +
       "/Submitted'>Resubmit</a></li";
    }

    if (Drupal.settings.nul_census_custom.censuspart == 'tabComplete') {
      var buttonsToAdd = "<li><a href='"
       + href  +
       "/Complete'>Mark Tab as Complete</a></li";
    }

    if (Drupal.settings.nul_census_custom.censuspart == 'civiSync') {
      var buttonsToAdd = "<li><a href='"
       + href  +
       "/civiSync'>Sync to Civi</a></li";
    }

    // Insert buttons to mark as reviewed and resubmit
    $($('ul.tabs.primary')).prepend(buttonsToAdd);
  });
}(jQuery));
