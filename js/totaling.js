(function ($) {
  // THIS JS handles tables with total fields that need to be calcualted
  // TODO add all table bases and total field Ids to this array for tables with one total
  var tableFieldBases = [
     {
      base: 'field-voter-',
      totalFieldId: 'edit-field-voter-societal-impact-und-0-value',
      totalBy: 'grand',
    },
    {
      base: 'field-advocacy-',
      totalFieldId: 'edit-field-advocacy-societal-impact-und-0-value',
      totalBy: 'grand',
    },
    {
      base: 'field-board-',
      totalFieldId: 'edit-field-board-member-grand-total-und-0-value',
      totalBy: 'race',
    },
    {
      base: 'field-police-',
      totalFieldId: 'edit-field-police-societal-impact-und-0-value',
      totalBy: 'grand',
    },
    {
      base: 'field-comm-org-',
      totalFieldId: 'edit-field-comm-org-societal-impact-und-0-value',
      totalBy: 'grand',
    },
    {
      base: 'field-forums-',
      totalFieldId: 'edit-field-forums-societal-impact-und-0-value',
      totalBy: 'grand',
    },
    {
      base: 'field-crja-',
      totalFieldId: 'edit-field-crja-societal-impact-und-0-value',
      totalBy: 'grand',
    },
    {
      base: 'field-public-',
      totalFieldId: {
        male: 'edit-field-public-total-male-und-0-value',
        female: 'edit-field-public-total-female-und-0-value',
      },
      totalBy: 'gender',
    },
    {
      base: 'field-direct-',
      totalFieldId: {
        male: 'edit-field-direct-total-male-und-0-value',
        female: 'edit-field-direct-total-female-und-0-value',
      },
      totalBy: 'gender',
    },
  ];
  var races = ['white', 'hispanic', 'asian-am', 'native-am', 'african-amer', 'other', 'african-am'];
  $(document).ready(function () {
    $(tableFieldBases).each(function (index, fieldBase) {
      // Disable total fields so user cannot enter content
      var totalFields = [
        fieldBase.totalFieldId.female,
        fieldBase.totalFieldId.male,
        fieldBase.totalFieldId,
      ];
      $(totalFields).each(function (index, field) {
        if (field && field.length > 0) {
          $('input#' + field).attr('readonly', true);
        }
      });

      $('input[id*="' + fieldBase.base + '"').keyup(function () {
        if (fieldBase.totalBy == 'race' || fieldBase.totalBy == 'grand') {
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
                if (fieldBase.totalBy == 'race') {
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
              }
            });

            $('input#' + fieldBase.totalFieldId).val(total);
          });
        }

        if (fieldBase.totalBy == 'gender') {
          maleTotal = parseInt(0);
          femaleTotal = parseInt(0);

          // Males
          if (this.id.indexOf('total') < 0 &&
          this.id.indexOf('male') > 0 &&
          this.id.indexOf('female') < 0 &&
          this.id.indexOf(fieldBase.base) > 0) {
            $('input[id*="' + fieldBase.base + '"').each(function (index, field) {
              if (field.id.indexOf('total') < 0 &&
              field.id.indexOf('male') > 0 &&
              this.id.indexOf('female') < 0) {
                if (parseInt($(field).val())) {
                  maleTotal = parseInt(maleTotal) + parseInt($(field).val());
                }
              }
            });

            $('input#' + fieldBase.totalFieldId.male).val(maleTotal);
          }

          // Females
          if (this.id.indexOf('total') < 0 &&
          this.id.indexOf('female') > 0 &&
          this.id.indexOf(fieldBase.base) > 0) {
            $('input[id*="' + fieldBase.base + '"').each(function (index, field) {
              if (field.id.indexOf('total') < 0 && field.id.indexOf('female') > 0) {
                if (parseInt($(field).val())) {
                  femaleTotal = parseInt(femaleTotal) + parseInt($(field).val());
                }
              }
            });

            $('input#' + fieldBase.totalFieldId.female).val(femaleTotal);
          }
        }
      });
    });
  });
}(jQuery));
