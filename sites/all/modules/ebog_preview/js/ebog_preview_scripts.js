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
          if (response.status == 'dl') {
            $('#dl-ebook').remove();
            $('body').append('<iframe id="dl-ebook" style="display:none;"></iframe>');
            $('#dl-ebook').attr('src', response.msg);
          }
          else {
            alert(response.msg);
          }
        }
      });

      return false;
    });
  });
})(jQuery);
