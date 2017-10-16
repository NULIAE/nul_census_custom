(function ($) {
  // TODO add all table bases and total field Ids to this array for tables with one total
  var tableFieldBases = [
     {
      base: 'field-voter-',
      totalFieldId: 'edit-field-voter-societal-impact-und-0-value',
    },
    {
      base: 'field-advocacy-',
      totalFieldId: 'edit-field-advocacy-societal-impact-und-0-value',
    },
    {
      base: 'field-board-',
      totalFieldId: 'edit-field-board-member-grand-total-und-0-value',
    },
    {
      base: 'field-police-',
      totalFieldId: 'edit-field-police-societal-impact-und-0-value',
    },
    {
      base: 'field-comm-org-',
      totalFieldId: 'edit-field-comm-org-societal-impact-und-0-value',
    },
    {
      base: 'field-forums-',
      totalFieldId: 'edit-field-forums-societal-impact-und-0-value',
    },
    {
      base: 'field-crja-',
      totalFieldId: 'edit-field-crja-societal-impact-und-0-value',
    },
  ];
  var races = ['white', 'hispanic', 'asian', 'native', 'african-amer', 'other'];
  $(document).ready(function () {
    $(tableFieldBases).each(function (index, fieldBase) {
      $('input[id*="' + fieldBase.base + '"').keyup(function () {
        var total = parseInt(0);
        $(races).each(function (index, race) {
          $('input[id*="' + fieldBase.base + race + '"').each(function (index, field) {
            // grand total
            if (this.id !== fieldBase.totalFieldId) {
              if (this.id.indexOf('total') < 0) {
                if (parseInt($(field).val())) {
                  total = parseInt(total) + parseInt($(field).val());
                }
              }

              // Total by race (for board)
              if (this.id.indexOf('total') > 0) {
                var totByRace = parseInt(0);

                if (parseInt($('input[id*="' + fieldBase.base + race + '-male"').val())) {
                  var totByRace = totByRace + parseInt(
                    $('input[id*="' + fieldBase.base + race + '-male"').val()
                  );
                }

                if (parseInt($('input[id*="' + fieldBase.base + race + '-female"').val())) {
                  var totByRace = totByRace + parseInt(
                    $('input[id*="' + fieldBase.base + race + '-female"').val()
                  );
                }

                $(this).val(totByRace);
              }
            }

          });

          $('input#' + fieldBase.totalFieldId).val(total);
        });
      });
    });
  });
}(jQuery));
