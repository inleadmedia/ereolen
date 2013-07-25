<?php

/**
 * @file
 * Plugin to provide access control/visibility based on length of
 * simplecontext argument (in URL).
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("Is app"),
  'description' => t('Access check based on user-agent string (eReolenApp).'),
  'callback' => 'ebog_offline_app_ctools_access_check',
  'settings form' => 'ebog_offline_app_ctools_access_settings',
  'summary' => 'ebog_offline_app_ctools_access_summary',
);

/**
 * Settings form for the 'by role' access plugin.
 */
function ebog_offline_app_ctools_access_settings(&$form, &$form_state, $conf) {

}

/**
 * Check for access.
 */
function ebog_offline_app_ctools_access_check($conf, $context) {
  return ebog_offline_is_app();
}

/**
 * Provide a summary description based upon the checked roles.
 */
function ebog_offline_app_ctools_access_summary($conf, $context) {
  return t('Only display when User-agent string (eReolenApp)');
}

