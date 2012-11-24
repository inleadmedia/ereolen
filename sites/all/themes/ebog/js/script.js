jQuery(function($){
  $(document).ready(function() {

    // Add truncation function to publisher description on object view.
    $('#ting-object .publisherDescription').condense({
        moreSpeed: 'fast',
        lessSpeed: 'fast',
        moreText: Drupal.t('More'),
        lessText: Drupal.t('Less'),
        ellipsis: '',
        condensedLength: 350
    });

    // Make the help block clickable.
    $('#block-block-12').click(function() {
      document.location.href = "/help";
    });

    // What dose this do ?
    $('.publizon-loans-list li:last-child').addClass('last');
  })
});
