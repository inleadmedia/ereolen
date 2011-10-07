jQuery(function($){

   $('.view-faq .item-list').each(function(){
     link = $(this).find('div.views-field-tid');
     $(this).find('div.views-field-tid').remove();
     $(this).append(link[0]);
  });

  $('#user-login #edit-name').blur(function(){
      jQuery.getJSON( '/getlibrary/'+$(this).val(),function(data){
        if(data.elib_library){
          $('#user-login #edit-library option[value='+data.elib_library+']').attr('selected','selected');
        }
      });
   });

});
