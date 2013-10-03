jQuery(function($){
  $(document).ready(function() {

    // Collapsible blocks
    var container = [
      '.pane-latest-news .pane-content',
      '.block-publizon_user .content',
      '.block-ebog_author_portrait .content',
      '.pane-embedvideo .pane-content',
      '.pane-front .pane-content'
    ];

    // Collapsible blocks
    function collapsibleBlocks() {
      for (var i = container.length - 1; i >= 0; i--) {
        if ($(container[i]).length !== 0) {
          var block = $(container[i]);

          // Make toggle feauture only for mobile screen
          if ( $(document).width() < 540 - (window.innerWidth - $(document).width()) ) {
            if (block.siblings('.collapsible-title').hasClass('active')) {
              block.show();
            } else {
              block.hide();
            }
            block.addClass('collapsible');
            block.siblings('h2').addClass('collapsible-title');
          } else {
            block.show().removeClass('collapsible');
            block.siblings('h2').removeClass('collapsible-title');
          }
        }
      }
    }

    // Init collapsible blocks
    collapsibleBlocks();

    // Check for changes on resize
    $(window).resize(function() {
      console.log(window.innerWidth - $(document).width());
      collapsibleBlocks();
    });

    // Toggle block state on click
    $('.collapsible-title').click(function () {
      var title = $(this);
      title.siblings('.collapsible').toggle();
      title.toggleClass('active');
    });

    // Make publisher description folderable.
    $('#ting-object .publisherDescription').expander({
      slicePoint:       350,
      expandPrefix:     ' ',
      expandText:       Drupal.t('More'),
      userCollapseText: Drupal.t('Less')
    });

    // Make the help block clickable.
    $('#block-block-12').click(function() {
      document.location.href = "/help";
    });

    // Add class "last" to book lists.
    $('.publizon-loans-list li:last-child').addClass('last');
    $('.display-book').parent().find('.display-book:last').addClass('last');

  });
});
