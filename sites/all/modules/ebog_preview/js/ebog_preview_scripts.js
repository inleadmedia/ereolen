(function ($) {
  $(document).ready(function() {
    $('a').filter(function(){
      var hr = new String($(this).attr('href'));

      // Check pattern
      if (hr.match(/ting\/object\/(.)+\/sample/)) {
        return true;
      }
    }).click(function() {
      href = $(this).attr('href');
      $.ajax({
        type : 'post',
        url : href + '/preview',
        dataType : 'json',
        success : function(response) {
          //alert(response);
        }
      });

      return false;
    });
  });
})(jQuery);
