jQuery(function($){
	 $('.view-faq .item-list').each(function(){
		 link = $(this).find('div.views-field-tid');
		 $(this).find('div.views-field-tid').remove();
		 $(this).append(link[0]);
	 });
});
