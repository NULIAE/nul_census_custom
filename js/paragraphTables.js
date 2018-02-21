(function ($) {
  // For some reason table on emergency_relief_activities needs extra js
  $(document).ready(function () {
    // Paragraphs to be reformatted

    var concatonateTables = function (tableClass) {
      var table;
      $('div.nulTable' + tableClass).each(function (index, value) {
        // This is the first row and will be used for the base table
        if (index == 0) {
          table = value;
        }

        if (index > 0) {
          console.log($(value).find('div.nulTableRow'));
          $(table).append($(value).find('div.nulTableRow'));
          $(value).hide();
        }
      });
    };

    var paragraphGroups = ['.group-funding-table', '.group-program-service-table'];
    $(paragraphGroups).each(function (index, className) {
      var updatedTable = concatonateTables(className);
    });

  });
}(jQuery));
