(function ($) {

  // TODO add all table bases and total field Ids to this array
  var tableFieldBases = [
     {
      base: 'field-voter-',
      totalFieldId: 'edit-field-voter-societal-impact-und-0-value',
    },
    {
      base: 'field-advocacy-',
      totalFieldId: 'edit-field-advocacy-societal-impact-und-0-value',
    },
  ];
  var races = ['white', 'hispanic', 'asian', 'native', 'african', 'other'];
  $(document).ready(function () {
    $(tableFieldBases).each(function (index, fieldBase) {
      $('input[id*="' + fieldBase.base + '"').keyup(function () {
        var total = parseInt(0);
        $(races).each(function (index, race) {
          $('input[id*="' + fieldBase.base + race + '"').each(function (index, field) {
            if (this.id !== fieldBase.totalFieldId) {
              if (parseInt($(field).val())) {
                total = parseInt(total) + parseInt($(field).val());
              }
            }
          });

          $('input#' + fieldBase.totalFieldId).val(total);
        });
      });
    });
  });
}(jQuery));
