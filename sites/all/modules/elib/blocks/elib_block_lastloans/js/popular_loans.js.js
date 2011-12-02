jQuery(document).ready(function($) {
  $('#ting-result #pager a').click(function() {
    var link = $(this).attr('href').split('#');

    $.scrollTo('#ting-search-results', 500);

    $.ajax({
      type: 'post',
      url: '/popular/page',
      data: { 'page': link },
      dataType: 'json',
      success: function(response) {
        if (response.status) {
          $('#ting-search-result ul').html(response.content);
        }
      }
    })

    return false;
  });
});
