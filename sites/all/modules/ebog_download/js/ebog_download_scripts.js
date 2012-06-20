(function ($) {
  $('.ebog-dlink').live('click', function() {
    $('#ting-download-popup-info').dialog('close');
  });

  var href = '';
  var clicked = null;
  var button = null;
  var popup_buttons = null;
  var ok_button = Drupal.t('Ok');
  var login_button = Drupal.t('Login');
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
        clicked.parent().find('.ajax-loader').remove();
        clicked.show();

        popup_buttons = {};
        popup_buttons[ok_button] = function() {
          button = $('#ting-download-popup').parents('.ui-dialog:first').find('button');
          button.css('visibility', 'hidden');
          button.parent().append('<div class="ajax-loader"></div>');
          process_loan();
        }

        popup_buttons[cancel_button] = function() {
          $('#ting-download-popup').dialog('close');
        }

        $('<div id="ting-download-popup" title="' + Drupal.t('Confirm reloan') + '">' + Drupal.t('Are you sure you want to reloan this item') + ' (<a href=' + '"' + '/faq/generelt-0#31n128' + '">' + Drupal.t('read more') + '</a>)?' + '</div>').dialog({
          modal : true,
          width: 'auto',
          height: 'auto',
          buttons: popup_buttons
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
          // Check if login dialog is open, if it is close it.
          var login_dialog = $('#ting-login-popup');
          if (login_dialog.length) {
            $('#ting-login-popup').dialog('close');
            $('#ting-login-popup').remove();
          }

          // Remove ajax loader
          $('#ting-download-popup').remove();
          clicked.parent().find('.ajax-loader').remove();
          clicked.show();

          if (response.status == false) {
            popup_buttons = {};
            popup_buttons[ok_button] = function() {
              $('#ting-download-popup').dialog('close');
            }

            $('<div id="ting-download-popup" title="' + response.title + '">' + response.content + '</div>').dialog({
              modal : true,
              width: 'auto',
              height: 'auto',
              buttons: popup_buttons
            });

            return;
          }
          else if (response.status === 'login') {
            // Login is required, so display login form.
            popup_buttons = {};

            // Hide login button from the form.
            var content = $(response.content);
            $('#edit-submit', content).remove();

            // Add action to the login dialog button.
            popup_buttons[login_button] = function() {
              // Add ajax loader to replace buttons.
              button = $('#ting-login-popup').parents('.ui-dialog:first').find('button');
              button.css('visibility', 'hidden');
              button.parent().append('<div class="ajax-loader"></div>');

              // Collect form values.
              var data = $('#elib-popup-login-form').formSerialize();

              // Make login ajax callback.
              $.ajax({
                type : 'POST',
                url : $('#elib-popup-login-form').attr('action'),
                dataType : 'json',
                data: data,
                success : function(response) {
                  // If not logged in handle errors.
                  if (response.status !== 'loggedin') {

                    // Display error message.
                    if ($('#ting-login-popup .messages').length) {
                      $('#ting-login-popup .messages').fadeOut('fast', function () {
                        $(this).remove();
                        $('#elib-popup-login-form #edit-name-wrapper').prepend(response.content);
                      });
                    }
                    else {
                      $('#elib-popup-login-form #edit-name-wrapper').prepend(response.content);
                    }

                    // Enable login buttons and remove ajax loader.
                    button.css('visibility', 'visible');
                    button.parent().find('.ajax-loader').remove();
                    return;
                  }
                  else {
                    // Try to process the loan once more.
                    process_loan();
                  }
                }
              });
              return false;

            }

            popup_buttons[cancel_button] = function() {
              // Close the form an remove it from the dom or close will not work
              // if displayed once more.
              $('#ting-login-popup').dialog('close');
              $('#ting-login-popup').remove();
            }

            options = {
              modal: true,
              width: 'auto',
              height: 'auto',
              buttons: popup_buttons
            }

            $('<div id="ting-login-popup" title="' + response.title + '">' + content[0].outerHTML + '</div>').dialog(options);

            return;
          }

          popup_buttons = {};
          popup_buttons[download_button] = function() {
            button = $('#ting-download-popup').parents('.ui-dialog:first').find('button');
            button.css('visibility', 'hidden');
            button.parent().append('<div class="ajax-loader"></div>');
            check_rules();
          }

          popup_buttons[cancel_button] = function() {
            // Close the form an remove it from the dom or close will not work
            // if displayed once more.
            $('#ting-download-popup').dialog('close');
            $('#ting-download-popup').remove();
          }


          $('<div id="ting-download-popup" title="' + response.title + '">' + response.content + '</div>').dialog({
            modal : true,
            width: 'auto',
            height: 'auto',
            buttons: popup_buttons
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
          dataType : 'json',
          success : function(response) {
            button.css('visibility', 'visible');
            button.parent().find('.ajax-loader').remove();
            $('#ting-download-popup').dialog('close');
            $('#ting-download-popup-info').remove();
            var options = {};
            if (response.status == false) {
              popup_buttons = {};
              popup_buttons[ok_button] = function() {
                $('#ting-download-popup-info').dialog('close');
              }

              options = {
                modal: true,
                width: 'auto',
                height: 'auto',
                buttons: popup_buttons
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

        popup_buttons = {};
        popup_buttons[ok_button] = function() {
          $('#ting-download-popup-error').dialog('close');
        }

        $('<div id="ting-download-popup-error" title="' + Drupal.t('Error') + '"><p>' + Drupal.t('Check all checkboxes') + '</p></div>').dialog({
          modal : true,
          width: 'auto',
          height: 'auto',
          buttons: popup_buttons
        });

        return true;
      }

      return false;
    }
  });
})(jQuery);
