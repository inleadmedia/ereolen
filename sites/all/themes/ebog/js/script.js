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

    // Swiped these blocks
    var swipe_array = [
      [
        '.feed-and-compare-front', // Parent
        '.feed_and_compare_item'   // Child
      ],
      [
        '.view-latest-news',
        '.views-row'
      ]
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
            }
            else {
              block.hide();
            }
            block.addClass('collapsible');
            block.siblings('h2').addClass('collapsible-title');
          }
          else {
            block.show().removeClass('collapsible');
            block.siblings('h2').removeClass('collapsible-title');
          }
        }
      }
    }

    // Activate swipe for given block
    // Block = [parent and child]
    function activate_swipe(block) {
      var parent = block[0],
          child = block[1],

          // Additional buttons for mouse click
          prev_slide = '<a href="#" class="prev_slide">prev</a>',
          next_slide = '<a href="#" class="next_slide">next</a>';

      if ( $(document).width() < 540 - (window.innerWidth - $(document).width()) ) {


        // Class for slyle purposes
        $(parent).addClass('swipe_container');
        $(child).addClass('swipe_item');

        // Attach buttons to parent
        if ($(parent).find('.prev_slide:first').length === 0) {
          $(prev_slide).appendTo($(parent));
          $(next_slide).appendTo($(parent));
        }

        // Show only first child in container
        $(parent).find(child).each(function (id, elem) {
          if (id !== 0) {
            $(elem).hide();
          } else {
            $(elem).addClass('visible_item');
          }
        });

        // Change slide on click
        // Next slide
        $(parent).find('.next_slide').click(function (e) {
          e.preventDefault();
          var triggers = $(parent).find(child),
              last_elem = $(parent).find(child).length-1,
              target;

          target = $(parent).find('.visible_item').prevAll().length;

          if (target === last_elem) {
            target = 0;
          } else {
            target++;
          }

          triggers.removeClass('visible_item').hide().eq(target).addClass('visible_item').show();
        });

        // Prev slide
        $(parent).find('.prev_slide').click(function (e) {
          e.preventDefault();
          var triggers = $(parent).find(child),
              first_elem = 0,
              last_elem = $(parent).find(child).length-1,
              target;

          target = $(parent).find('.visible_item').prevAll().length;

          if (target === first_elem) {
            target = last_elem;
          } else {
            target--;
          }

          triggers.removeClass('visible_item').hide().eq(target).addClass('visible_item').show();
        });

        // Change slide on swipe
        $(parent).touchwipe({
          wipeRight: function() {
            var triggers = $(parent).find(child),
              last_elem = $(parent).find(child).length-1,
              target;

            target = $(parent).find('.visible_item').prevAll().length;

            if (target === last_elem) {
              target = 0;
            } else {
              target++;
            }

            triggers.removeClass('visible_item').hide().eq(target).addClass('visible_item').show();
          },
          wipeLeft: function() {
            var triggers = $(parent).find(child),
              first_elem = 0,
              last_elem = $(parent).find(child).length-1,
              target;

            target = $(parent).find('.visible_item').prevAll().length;

            if (target === first_elem) {
              target = last_elem;
            } else {
              target--;
            }

            triggers.removeClass('visible_item').hide().eq(target).addClass('visible_item').show();
          },
          min_move_x: 20,
          min_move_y: 20,
          preventDefaultEvents: true
        });
      }
    }

    // Send each swipe block to activation function
    function swipe_blocks() {
      for (var i = swipe_array.length - 1; i >= 0; i--) {
        activate_swipe($(swipe_array[i]));
      }
    }

    // Init collapsible blocks
    collapsibleBlocks();
    swipe_blocks();

    // Check for changes on resize
    $(window).resize(function() {
      collapsibleBlocks();
      swipe_blocks();
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
