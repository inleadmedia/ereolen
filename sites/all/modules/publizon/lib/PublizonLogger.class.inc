<?php

abstract class PublizonLogger {

  const EMERGENCY = 'EMERGENCY';
  const ALERT = 'ALERT';
  const CRITICAL = 'CRITICAL';
  const ERROR = 'ERROR';
  const WARNING = 'WARNING';
  const NOTICE = 'NOTICE';
  const INFO = 'INFO';
  const DEBUG = 'DEBUG';

  static public $levels = array(self::EMERGENCY, self::ALERT, self::CRITICAL, self::ERROR,
                                self::WARNING,  self::NOTICE, self::INFO, self::DEBUG);

  /**
   * Log a message.
   *
   * @param string $message The message to log
   * @param string $severity The severity of the message
   */
  public function log($message, $severity = self::INFO) {
    if (!in_array($severity, self::$levels)) {
      throw new Exception('Unsupported severity: '.$severity);
    }
    $this->doLog($message, $severity);
  }

  abstract protected function dolog($message, $severity);
}

class PublizonWatchDogLogger extends PublizonLogger {
  public function doLog($message, $severity) {
    watchdog('publizon client', htmlspecialchars($message, ENT_QUOTES, 'UTF-8', FALSE), array(),
             constant('WATCHDOG_' . $severity),
             'http://' . $_SERVER["HTTP_HOST"] . request_uri());
  }
}

class PublizonVoidLogger extends PublizonLogger {
  public function doLog($message, $severity) {}
}