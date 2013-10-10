jQuery(function($) {
  "use strict";
  $(document).ready(function() {

    // Collapse listed blocks in mobile view
    var blocksBeingCollapsed = [
      '.pane-latest-news .pane-content',
      '.block-publizon_user .content',
      '.block-ebog_author_portrait .content',
      '.pane-embedvideo .pane-content',
      '.pane-front .pane-content'
    ];

    // Rotate listed blocks in mobile view
    var blocksBeingRotated = [
      [
        '.feed-and-compare-front',  // Items container
        '.feed_and_compare_item'    // Items
      ],
      [
        '.view-latest-news',
        '.views-row'
      ],
      [
        '#ting-search-result > ul',
        'li.ting-collection'
      ],
      [
        '.publizon-loans-list',
        '.display-book'
      ],
      [
        '.publizon-products-list',
        '.display-book'
      ],
      [
        '.view-faq',
        '.item-list'
      ]
    ];

    /**
     * Make block collapsible and expandable
     * @param  object blockSelector
     *   jQuery selector for block which will be collapsed
     */
    function makeBlockCollapsible(block) {
      // Check block previous state "collapsed or expanded" if window resized
      if (block.siblings('.collapsible-title').is('.expanded')) {
        block.show();
      }
      else {
        block.hide();
      }
      block.addClass('collapsible');
      block.siblings('h2').addClass('collapsible-title');
    }

    /**
     * Slide to next item
     * @param  object items
     *   List of items
     * @param  object lastItem
     *   Last item in given items list
     */
    function goToNextItem(items, lastItem) {
      var currentItem = items.parent().find('.visible_item').prevAll().length;

      if (currentItem === lastItem) {
        currentItem = 0;
      }
      else {
        currentItem++;
      }

      items.removeClass('visible_item').hide().eq(currentItem).addClass('visible_item').show();
    }

    /**
     * Slide to previous item
     * @param  object items
     *   List of items
     * @param  object lastItem
     *   Last item in given items list
     */
    function goToPrevItem(items, lastItem) {
      var currentItem = items.parent().find('.visible_item').prevAll().length;

      if (currentItem === 0) {
        currentItem = lastItem;
      }
      else {
        currentItem--;
      }

      items.removeClass('visible_item').hide().eq(currentItem).addClass('visible_item').show();
    }

    /**
     * Activate item Rotation for given block.
     *
     * @param object itemsContainer
     *   Items container block
     * @param object items
     *   List of items
     */
    function activateRotation(itemsContainer, items) {
      // Additional buttons to rotate items by click
      var prevItem = '<a href="#" class="prev_item">prev</a>',
          nextItem = '<a href="#" class="next_item">next</a>',

          // Last item
          lastItem = items.length - 1;

      // Classes for CSS purposes
      itemsContainer.addClass('rotated_container');
      items.addClass('rotated_item');

      // Append buttons to items container
      if (itemsContainer.find('.prev_item:first').length === 0) {
        $(prevItem).appendTo(itemsContainer);
        $(nextItem).appendTo(itemsContainer);
      }

      // Show buttons if they were hidden on resize
      itemsContainer.find('.prev_item:first').show();
      itemsContainer.find('.next_item:first').show();

      // Show only first item in items container
      items.each(function (i, elem) {
        if (i !== 0) {
          $(elem).hide();
        }
        else {
          $(elem).addClass('visible_item');
        }
      });

      // Change to next item on click
      itemsContainer.find('.next_item:first').click(function(e) {
        e.preventDefault();
        goToNextItem(items, lastItem);
      });

      // Change to previous item on click
      itemsContainer.find('.prev_item:first').click(function(e) {
        e.preventDefault();
        goToPrevItem(items, lastItem);
      });

      // Change item on swipe
      itemsContainer.touchwipe({
        wipeRight: function() {
          goToNextItem(items, lastItem);
        },
        wipeLeft: function() {
          goToPrevItem(items, lastItem);
        },
        min_move_x: 20,
        min_move_y: 20,
        preventDefaultEvents: true
      });
    }

    /**
     * Checks if mobile device is active
     * @param  int width
     *   Maximum mobile width
     */
    function checkIfMobile(width) {
      var mobileWidth = $(document).width() <= width - (window.innerWidth - $(document).width()),
        mobileDevice = /Android|webOS|iPhone|iPad|iPod|BlackBerry|IEMobile|Opera Mini/i.test(navigator.userAgent);

      if (mobileWidth || mobileDevice) {
        return true;
      }
    }

    /**
     * Execute listed features
     */
    function runMobileFeatures() {
      // Collapse block if it's available
      $(blocksBeingCollapsed).each(function (i, block) {
        if ($(block).length !== 0) {
          makeBlockCollapsible($(block));
        }
      });

      // Rotate block if it's available
      $(blocksBeingRotated).each(function (i, block) {
        if ($(block[0]).length !== 0) {
          activateRotation($(block[0]), $(block[0]).find(block[1]));
        }
      });
    }

    /**
     * Revert mobile features
     */
    function revertMobileFeatures() {
      // Make block expandable if they collapsed
      $(blocksBeingCollapsed).each(function (i, block) {
        $(block).show().removeClass('collapsible').siblings('h2').removeClass('collapsible-title');
      });

      // Revert blocks rotation
      $(blocksBeingRotated).each(function (i, block) {
        if ($(block[0]).length !== 0) {
          var itemsContainer = $(block[0]),
              items = $(block[0]).find(block[1]);

          items.show();
          itemsContainer.find('.prev_item').hide();
          itemsContainer.find('.next_item').hide();
        }
      });
    }

    // Features available for mobile device only
    var isMobile = checkIfMobile(540);

    // Activate or revert mobile features
    if (isMobile) {
      runMobileFeatures();
    }
    else {
      revertMobileFeatures();
    }

    // Check for changes on resize
    $(window).resize(function() {
      isMobile = checkIfMobile(540);

      // Activate mobile features
      if (isMobile) {
        runMobileFeatures();
      }
      else {
        revertMobileFeatures();
      }
    });

    // Toggle block state on click
    $('.collapsible-title').live("click", function() {
      var title = $(this);

      title.siblings('.collapsible').toggle();
      title.toggleClass('expanded');
    });

    // Make publisher description folderable.
    $('#ting-object .publisherDescription').expander({
      slicePoint: 350,
      expandPrefix: ' ',
      expandText: Drupal.t('More'),
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
