function Reader(settings) {

    if (typeof settings.elementId == 'undefined')
        throw 'Argument elementId is required.';

    if (typeof settings.orderId == 'undefined')
        throw 'Argument orderId is required.';

    if (typeof settings.sessionKeyUrl == 'undefined')
        throw 'Argument sessionKeyUrl is required.';

    var bookmarker;
    var bookRenderer;
    var zoomer;
    var toc;
    var spinner;
    var modalWindow;
    var jq17;

    // Styles MUST be inline - otherwise epub css might override the styles
    var defaultMarkup = '<div id="' + ElementId.ReaderHeader + '">' +
        '<a href="javascript:reader.ZoomOut();" id="reader-link-zoomout"><img src="https://reader.pubhub.dk/images/1.1.2/zoom_out.png" title="Zoom ud" border="0" style="width:42px;height:42px;" /></a>' +
        '<a href="javascript:reader.ZoomNormal();" id="reader-link-zoomnormal"><img src="https://reader.pubhub.dk/images/1.1.2/zoom_normal.png" title="Zoom normal" border="0" style="margin-left:10px;width:42px;height:42px;" /></a>' +
        '<a href="javascript:reader.ZoomIn();" id="reader-link-zoomin"><img src="https://reader.pubhub.dk/images/1.1.2/zoom_in.png" title="Zoom in" border="0" style="margin-left:10px;width:42px;height:42px;" /></a>' +
        '<a href="javascript:reader.ToggleBookmark();"><img src="https://reader.pubhub.dk/images/1.1.2/bookmark.png" title="Tilføj/fjern bogmærke" border="0" style="margin-left:10px;width:42px;height:42px;" /></a>' +
        '<a href="javascript:reader.DisplayAddNoteWindow();"><img src="https://reader.pubhub.dk/images/1.1.2/note.png" title="Tilføj/ret note" border="0" style="margin-left:10px;width:42px;height:42px;" /></a>' +
        '<a href="javascript:reader.DisplayBookmarkList();"><img src="https://reader.pubhub.dk/images/1.1.2/bookmarks_notes.png" title="Vis bogmærker & noter" border="0" style="margin-left:10px;width:42px;height:42px;" /></a>' +
        '<a href="javascript:reader.DisplayToc();"><img src="https://reader.pubhub.dk/images/1.1.2/toc.png" title="Indholdsfortegnelse" border="0" style="margin-left:10px;width:42px;height:42px;" /></a>' +
        '</div>' +
        '<div id="reader-navigation-top">' +
        '<a href="javascript:reader.PrevPage();"><img src="https://reader.pubhub.dk/images/arrow_left.png" alt="Forrige side" border="0" style="left:0px;position:absolute;margin:10px;width:17px;height:30px;"/></a>' +
        '<div id="reader-bookmark-mobile" onclick="javascript:reader.RemoveBookmark();"></div>' +
        '<div id="reader-note-mobile" onclick="javascript:reader.RemoveNote();"></div>' +
        '<a href="javascript:reader.NextPage();"><img src="https://reader.pubhub.dk/images/arrow_right.png" alt="Næste side" border="0" style="right:0px;position:absolute;margin:10px;width:17px;height:30px;" /></a>' +
        '</div>' +
        '<div id="reader-content-container">' +
        '<div id="reader-content">' +
        '<div id="reader-navigation-left" style="text-align:left;">' +
        '<a href="#" id="reader-button-previouspage" onclick="javascript:reader.PrevPage()">' +
        '<img src="https://reader.pubhub.dk/images/arrow_left.png" alt="Forrige side" id="reader-button-previouspage-image" border="0" style="width:17px;height:30px;" />' +
        '</a>' +
        '</div>' +
        '<div id="reader-book"><div id="reader-book-inner-div" i="0"></div></div>' +
        '<div id="reader-navigation-right" style="text-align:left;">' +
        '<img src="https://reader.pubhub.dk/images/bookmark.png" id="reader-bookmark" alt="Fjern bogmærke" onclick="javascript:reader.RemoveBookmark();"  style="width:55px;height:75px;" />' +
        '<img src="https://reader.pubhub.dk/images/note.png" id="reader-note" alt="Fjern note" onclick="javascript:reader.RemoveNote();"  style="width:55px;height:75px;" />' +
        '<a href="#" id="reader-button-nextpage" onclick="javascript:reader.NextPage();">' +
        '<img src="https://reader.pubhub.dk/images/arrow_right.png" alt="Næste side" id="reader-button-nextpage-image" border="0" style="width:17px;height:30px;" />' +
        '</a>' +
        '</div>' +
        '</div>' +
        '<div id="reader-modal-window">' +
        '<div id="reader-modal-window-header">' +
        '<span id="reader-modal-window-header-title"></span>' +
        '<a id="reader-modal-window-button-close" href="javascript:reader.CloseModalWindow();"><img src="https://reader.pubhub.dk/images/close.png" border="0" alt="Luk" style="width:13px;height:12px;" /></a>' +
        '</div>' +
        '<div id="reader-modal-window-content"></div>' +
        '</div>' +
        '</div>' +
        '<span id="reader-rendertime" style="display:none;"></span>';

    var numberOfSuccessCallbacksCalled = 0;
    this.Display = function() {
        LoadDependencies();
    }

    var numberOfDependenciesLoaded = 0;
    var numberOfDependeciesToBeLoaded;

    function LoadDependencies() {
        numberOfDependeciesToBeLoaded = 13;

        LoadScript('//ajax.googleapis.com/ajax/libs/jquery/1.7.2/jquery.min.js', function () {
            window.jq17 = jQuery.noConflict(true);
            jq17 = window.jq17;

            LoadDependency('jquery.browser.mobile.js', true);
            LoadDependency('scripts/spin.min.js', false);
            LoadDependency('jquery.cj-swipe.js', true);
            LoadDependency('scripts/reader-1.1.4/browser.js', false);
            LoadDependency('scripts/reader-1.1.4/serviceurl.js', false);
            LoadDependency('scripts/reader-1.1.4/toc.js', false);
            LoadDependency('scripts/reader-1.1.4/modalwindow.js', false);
            LoadDependency('scripts/reader-1.1.4/security.js', false);
            LoadDependency('scripts/reader-1.1.4/zoomer.js', false);
            LoadDependency('scripts/reader-1.1.4/epubloader.js', false);
            LoadDependency('scripts/reader-1.1.4/bookrenderer.js', false);
            LoadDependency('scripts/reader-1.1.4/bookmarker.js', false);
            LoadDependency('scripts/reader-1.1.4/readerspinner.js', false);
//            LoadDependency('scripts/reader-1.1.4/reader-1.1.4.css');
        });
    }

    function LoadDependency(path, local) {
        var extension = path.substring(path.lastIndexOf('.'));

        if (extension == '.js') {
            LoadScript(GetReaderBaseUri(local) + path, function() {
                numberOfDependenciesLoaded++;
                if (numberOfDependenciesLoaded == numberOfDependeciesToBeLoaded) SetSessionKeyAndInitReader();
            });
        }

        if (extension == '.css') {
            LoadStylesheet(function() {
                numberOfDependenciesLoaded++;
                if (numberOfDependenciesLoaded == numberOfDependeciesToBeLoaded) SetSessionKeyAndInitReader();
            }, GetReaderBaseUri() + path);
        }
    }

    function LoadStylesheet(onload, href) {
        var id = href.substring(href.lastIndexOf('/') + 1, href.lastIndexOf('.'));
        jq17.get(href, function (response) {
            if (!jq17('#' + id).length) jq17('head').append('<style id="' + id + '">' + response + '</style>');
            onload();
        });
    }

    function LoadScript(url, success) {
        var script = document.createElement('script');

        script.src = url;
        var head = document.getElementsByTagName('head')[0];
        var done = false;

        // Attach handlers for all browsers
        script.onload = script.onreadystatechange = function() {
            if (!done && (!this.readyState || this.readyState == 'loaded' || this.readyState == 'complete')) {
                done = true;
                success();
                script.onload = script.onreadystatechange = null;
                head.removeChild(script);
            }
        };

        head.appendChild(script);
    }

    function SetSessionKeyAndInitReader() {
        spinner = new ReaderSpinner(settings.elementId);
        spinner.Start();

        ServiceUrl.SetSessionKeyUrl(settings.sessionKeyUrl);
        ServiceUrl.SetSessionKey(settings.sessionKeyUrl, function() {
            Init();
        });
    }

    function Init() {
        new Security().PreventCopy();

        jq17('#' + settings.elementId).append(defaultMarkup);

        var $readerBook = jq17('#reader-book');
        var $readerHeader = jq17('#reader-header');

        if (jq17.browser.mobile) {
            // Hide left navigation
            jq17('#reader-navigation-left').css('display', 'none');
            jq17('#reader-book').css('left', '0');

            // Hide right navigation
            jq17('#reader-navigation-right').css('display', 'none');
            jq17('#reader-book').css('right', '0');

            jq17('#reader-link-zoomout').css('display', 'none');
            jq17('#reader-link-zoomnormal').css('display', 'none');
            jq17('#reader-link-zoomin').css('display', 'none');

            jq17('#reader-modal-window').css('height', $readerBook.height() - 10 + 'px');
            jq17('#reader-modal-window-content').css('height', ($readerBook.height() - 46) + 'px');

            ActivateMenuClick();

            $readerHeader.delay(5000).slideUp();
        } else {
            jq17('#reader-modal-window').css('top', '10%');
        }

        var streamingServiceBaseUri = GetStreamingServiceBaseUri();

        bookmarker = new Bookmarker(settings.orderId);
        toc = new Toc(streamingServiceBaseUri, settings.orderId);
        bookRenderer = new BookRenderer(settings.orderId, bookmarker, spinner);
        zoomer = new Zoomer($readerBook, bookRenderer);
        modalWindow = new ModalWindow();

        bookRenderer.InitNavigationKeys();

        bookmarker.LoadBookmarks(function() {
            bookRenderer.DisplayByReadingState();
        });

        counter = 0;
    }

    SetSessionKey = function(sessionKeyUrl, callback) {
        jq17.ajax({
            url: sessionKeyUrl,
            dataType: 'json',
            success: function(data) {
                ServiceUrl.SessionKey = data.SessionId;
                callback();
            },
            error: function(xhr, textStatus, errorThrown) {
                alert(xhr.status);
            }
        });
    }

    this.DisplayChapter = function(index) {
        this.CloseModalWindow();
        bookRenderer.DisplayHtmlPage(index);
    }

    this.NextPage = function() {
        bookRenderer.Next();
    }

    this.PrevPage = function() {
        bookRenderer.Back();
    }

    this.ZoomIn = function() {
        zoomer.ZoomIn();
    }

    this.ZoomOut = function() {
        zoomer.ZoomOut();
    }

    this.ZoomNormal = function() {
        zoomer.ZoomNormal();
    }

    this.AddBookmark = function() {
        bookmarker.AddBookmark(bookRenderer.GetCurrentHtmlPageIndex(), bookRenderer.IndexOfFirstElementOnPage(), bookRenderer.GetFirstXCharactersOfCurrentPage(bookmarker.numberOfCharactersInBookmarkDescription));
    }

    this.ToggleBookmark = function() {
        bookmarker.ToggleBookmark(bookRenderer.GetCurrentHtmlPageIndex(), bookRenderer.IndexOfFirstElementOnPage(), bookRenderer.GetFirstXCharactersOfCurrentPage(bookmarker.numberOfCharactersInBookmarkDescription));
        HideMenu();
    }

    this.RemoveBookmark = function() {
        bookmarker.RemoveBookmark();
    }

    this.RemoveNote = function() {
        bookmarker.RemoveNote();
    }

    this.DisplayAddNoteWindow = function() {
        HideMenuAndDeactivateMenuClick();
        bookmarker.DisplayAddNoteWindow(bookRenderer.GetCurrentHtmlPageIndex(), bookRenderer.IndexOfFirstElementOnPage(), bookRenderer.IndexOfLastElementOnPage(), modalWindow);
    }

    this.AddNote = function(text) {
        bookmarker.AddNote(bookRenderer.GetCurrentHtmlPageIndex(), bookRenderer.IndexOfFirstElementOnPage(), $('#note-text').val());
        this.CloseModalWindow();
    }

    this.UpdateNote = function(markId, text) {
        bookmarker.UpdateNote(markId, $('#note-text').val());

        // Prevent that the top menu slides down on mobile devices
        if (window.event && window.event.stopPropagation) // stopPropagation doesn't work in IE
            window.event.stopPropagation();

        this.CloseModalWindow();
    }

    this.DisplayBookmarkList = function() {
        toc.GetToc({
            success: function(data) {
                bookmarker.DisplayBookmarkList(data, modalWindow);
            }
        });

        HideMenuAndDeactivateMenuClick();
    }

    this.GoToBookmark = function(bookmarkId) {
        var bookmark = bookmarker.GetBookmarkById(bookmarkId);
        if (bookmark != null)
            bookRenderer.DisplayByHtmlPageIndexAndElementIndex(bookmark.chapterIndex, bookmark.elementIndex);
        else
            alert('Bogmærket blev ikke fundet.');
        this.CloseModalWindow();
    }

    this.CloseModalWindow = function() {
        ActivateMenuClick();
        modalWindow.Close();
    }

    this.DisplayToc = function() {
        toc.DisplayToc(modalWindow);
        HideMenuAndDeactivateMenuClick();
    }

    function HideMenuAndDeactivateMenuClick() {
        HideMenu();
        DeactivateMenuClick();
    }

    function HideMenu() {
        if (jq17.browser.mobile)
            jq17('#reader-header').slideUp();
    }

    function ShowMenu() {
        if (jq17.browser.mobile)
            jq17('#reader-header').slideDown();
    }

    function DeactivateMenuClick() {
        if (jq17.browser.mobile)
            jq17('#reader-content-container').unbind('click');
    }

    function ActivateMenuClick() {
        if (jq17.browser.mobile) {
            $('#reader-content-container').bind('click', function() {
                var $readerHeader = jq17('#reader-header');
                if ($readerHeader.css('display') == 'none')
                    $readerHeader.slideDown();
                else
                    $readerHeader.slideUp();
            });
        }
    }

    function GetStreamingServiceBaseUri() {
        if (window.location.hostname == 'localhost')
            return 'http://localhost:15495/publicstreaming/v1/';
        return 'https://streaming.pubhub.dk/publicstreaming/v1/';
    }

    function GetReaderBaseUri(local) {
        if (local) {
          return '/sites/all/modules/ebog_stream/js/';
        }
        if (window.location.hostname == 'localhost')
            return 'http://localhost:64286/';
        return 'https://reader.pubhub.dk/';
    }

};


function ElementId() {
}

ElementId.ReaderHeader = 'reader-header';