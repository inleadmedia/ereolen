<?php
/**
 * @file
 * Defines the comunication client to use with the Publizon client. It makes
 * some overrides of the default NanoSOAPClient implementation.
 */

/**
 * Implementation of the communication object based on NanoSOAPClient object.
 */
class PublizonNanoSOAPClient extends NanoSOAPClient {

  /**
   * Default construtor.
   */
  function __construct($endpoint, $options = array()) {
    parent::__construct($endpoint, $options);
  }

  /**
   * Make a SOAP request. Modified this verion to new set a SOAP action header
   * as publizon's service don't link that
   *
   * @param array $parameters
   *   The parameters to send with the SOAP request.
   *
   * @return string
   *   The SOAP response.
   */
  function call($action, $parameters = array()) {
    // Set content type and send the SOAP action as a header.
    $headers = array(
      'Content-Type: text/xml',
    );

    // Make a DOM document from the envelope and get the Body tag so we
    // can add our request data to it.
    $this->doc = new DOMDocument();
    $this->doc->loadXML($this->generateSOAPenvelope());
    $body = $this->doc->getElementsByTagName('Body')->item(0);

    // Convert the parameters into XML elements and add them to the
    // body. The root element of this structure will be the action.
    $elem = $this->convertParameter($action, $parameters);
    $body->appendChild($elem);

    // Render and store the final request string.
    $this->requestBodyString = $this->doc->saveXML();

    // Send the SOAP request to the server via CURL.
    return $this->curlRequest($this->endpoint, 'POST', $this->requestBodyString, $headers);
  }
}
