<?php

// Override ting object page title
drupal_set_title(check_plain($vars['object']->title . ' ' . t('af') . ' ' . $vars['object']->creators_string));

/* Proces OpenGraph tags into head if we are located on a itemPage */
global $base_url;
$head = '';
$isbn = preg_replace('/[^0-9]+/', '', $vars['object']->record['dc:identifier']['dkdcplus:ISBN'][0]);

$head .= '<meta property="og:url" content="' . $base_url .'/ting/object/' . $vars['object']->id . '" />' . "\n";
$head .= '<meta property="og:type" content="book" />' . "\n";
$head .= '<meta property="og:title" content="' . $vars['object']->title .'" />' . "\n";
$head .= '<meta property="og:image:secure_url" content="' . elib_book_cover($isbn, '170_x') . '" />' . "\n";
$head .= '<meta property="og:site_name" content="eReolen.dk" />' . "\n";
$head .= '<meta property="fb:admins" content="694811338" />' . "\n";
$head .= '<meta property="og:author" content="' . $vars['object']->creators_string . '" />' . "\n";
$head .= '<meta property="og:isbn" content="' . $isbn . '" />' . "\n";
$head .= '<meta property="og:release_date" content="' . $vars['object']->date . '" />';

drupal_set_html_head($head);
