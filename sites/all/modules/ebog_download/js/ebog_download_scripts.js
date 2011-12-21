(function ($) {
  $('.ebog-dlink').live('click', function() {
    $('#ting-download-popup-info').dialog('close');
  });
  
  var href = '';
  var clicked = null;
  var button = null;
  var ok_button = Drupal.t('Ok');
  var cancel_button = Drupal.t('Cancel');
  var download_button = Drupal.t('Proceed to download');

  // Handle clicked loan link, those matching 'ting/object/%/download' pattern
  $(document).ready(function() {
    $('a').live('click', function() {
      href = $(this).attr('href');
      if (!href.match(/ting\/object\/(.)+\/download/)) {
        return true;
      }

      clicked = $(this);
      clicked.parent().find('.ajax-loader').remove();
      clicked.hide();
      clicked.parent().append('<div class="ajax-loader"></div>');

      if (clicked.hasClass('re-loan')) {
        $('#ting-download-popup').remove();
        $('<div id="ting-download-popup" title="' + Drupal.t('Confirm reloan') + '">' + Drupal.t('Are you sure you want to reloan this item?') + '</div>').dialog({
          modal : true,
          width: 'auto',
          height: 'auto',
          buttons: {
            ok_button : function() {
              button = $('#ting-download-popup').parents('.ui-dialog:first').find('button');
              button.css('visibility', 'hidden');
              button.parent().append('<div class="ajax-loader"></div>');
              process_loan();
            },
            cancel_button : function() {
              $('#ting-download-popup').dialog('close');
              clicked.parent().find('.ajax-loader').remove();
              clicked.show();
            }
          }
        });
      }
      else {
        process_loan();
      }

      return false;
    });

    // Process loan/reloan/download
    var process_loan = function() {
      $.ajax({
        type : 'post',
        url : href + '/popup',
        dataType : 'json',
        success : function(response) {
          $('#ting-download-popup').remove();
          clicked.parent().find('.ajax-loader').remove();
          clicked.show();

          if (response.status == false) {

            $('<div id="ting-download-popup" title="' + response.title + '">' + response.content + '</div>').dialog({
              modal : true,
              width: 'auto',
              height: 'auto',
              buttons: {
                ok_button : function() {
                  $('#ting-download-popup').dialog('close');
                }
              }
            });

            return;
          }

          $('<div id="ting-download-popup" title="' + response.title + '">' + response.content + '</div>').dialog({
            modal : true,
            width: 'auto',
            height: 'auto',
            buttons: {
              download_button : function() {
                button = $('#ting-download-popup').parents('.ui-dialog:first').find('button');
                button.css('visibility', 'hidden');
                button.parent().append('<div class="ajax-loader"></div>');
                check_rules();
              }
            }
          });
        }
      });
    }

    // Check those checkboxes
    var check_rules = function() {
      var boxes = $('.ebog-download-confirm').find('input[type=checkbox]');
      if (boxes.length == boxes.filter(':checked').length) {
        $.ajax({
          type : 'post',
          url : href + '/request',
          data : {'dload_only' : clicked.hasClass('re-download') ? 1 : 0},
          dataType : 'json',
          success : function(response) {
            button.css('visibility', 'visible');
            button.parent().find('.ajax-loader').remove();
            $('#ting-download-popup').dialog('close');
            $('#ting-download-popup-info').remove();
            var options = {};
            if (response.status == false) {
              options = {
                modal: true,
                width: 'auto',
                height: 'auto',
                buttons: {
                  ok_button : function() {
                    $('#ting-download-popup-info').dialog('close');
                  }
                }
              }
            }
            else {
              options = {
                modal: true,
                width: 'auto',
                height: 'auto'
              }
            }
            
            $('<div id="ting-download-popup-info" title="' + response.title + '">' + response.content + '</div>').dialog(options);
          }
        });
      }
      else {
        button.css('visibility', 'visible');
        button.parent().find('.ajax-loader').remove();
        $('#ting-download-popup-error').remove();
        $('<div id="ting-download-popup-error" title="' + Drupal.t('Error') + '"><p>' + Drupal.t('Check all checkboxes') + '</p></div>').dialog({
          modal : true,
          width: 'auto',
          height: 'auto',
          buttons: {
            ok_button : function() { $(this).dialog('close'); }
          }
        });

        return true;
      }

      return false;
    }
  });
})(jQuery);
