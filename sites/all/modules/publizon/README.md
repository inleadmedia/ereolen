This is the module used for communication with the Publizon web-service, but it
also contains theme function for constructing list of PublizonProduct objects
etc.

And all communication with Publizon should go through the objects provide by
this module and its sub-modules. To ensure that all communication is done in the
same way and the objects also takes advantages of caching, so to get the best
performance this is the gateway to Publizon.

The module also implements drush support for clearing the different caches,
without having to wipe the whole site.

_Note_: You should notice that the Publizon object uses exception as error
handle and an uncut exception will give the user the WSOD. So you should use
try ... catch blocks.

# Basic usage
The main module implements the PublizonClient and Logger classes and the
communication link to NanoSOAP. The module uses NanoSOAP because its fast and it
is already used by the ting client and in contrast to PHP build in SOAP client
it does not crash the site when Publizon's WSDL is not available.

The PublizonClient object is a single-ton pattern and you can not create an
instance directly.

The Drupal parts of the modules contains different helper functions that can be
used to check different states an extract information. So please look into the
module for more information about these helper functions.

## Usage
If you need to communicate directly with Publizon the client can be fetched an
instance of the object link this:

  $client = PublizonClient::getClient($retailer_id, $retailer_key_code, $langcode, $logger);

Please not that all parameters are optional and that it's only in very special
cases that you would need to use any parameters. The object it self will detect
if a user is logged in (active session) and if not fallback to the default
configuration provided by the site administrator. So basically you should only
use parameters, if you want to override the default behaviour (mostly for
testing).

_Note_: Not all Publizon API class have been implemented yet and they will throw
 a "Not implemented yet" exception. See the PublizonClient.class.inc for more
information.

# Publizon Product
If you simply need to get information about a product from Publizon use the
PublizonProduct class, it will make the connection for you. It will also fetch
the required TingObject from the data well when it's need (gets loaded when
requested the first time).

## Usage
To get a product simply crate a new PublizonProduct object.

  $product = PublizonProduct($isbn);

## Methods
To get a cover image use the getCover method on the Product class. The method
takes an Image Cache preset name as parameter to return the URL to the required
image. It takes usage of the fact that the cover may have been downloaded before
and uses the local copy. If you which to download a new version the second
argument should be set to FALSE (default TRUE).

  $product->getCover('presetName');

To get a ting object for the product simply call:

  $ting_object = $product->getTingObject()

# Publizon Loans
If you need access to the currently logged in users loans create a new
PublizonUserLoans object. It will contain the loans and basic information about
the users loan limits etc.

_Note_: The users loan limits are cached together with the loans and is only
updated when the user changes his/her loans (new loan). So you can not use all
the timestamps provided by Publizon (eg. next loan period) and you should check
if a given loan is expire before displaying it.

## Usage
Simply create a new object and theme it as a list:

  if ($uid = publizon_user_is_logged_in()) {
    try {
      $publizon_loans = new PublizonUserLoans($uid, TRUE);
      $content = theme('publizon_loans_list', $publizon_loans->loans);
    }
    catch (Exception $e) {
      drupal_set_message($e->getMessage(), 'error');
    }
  }

The loans are available as PublizonLoan objects like this:

  $loans = $publizon_loans->loans;
  $loan = $publizon_loans[$isbn];

Both the PublizonUserLoans and PublizonLoan objects have helper function to
create expire strings and check for different conditions.

  // PublizonUserLoans.
  $publizon_loans->isLoan($isbn, FALSE);
  $publizon_loans->createLoan($isbn);
  $publizon_loans->nextLoanPeriode();

  // PublizonLoan.
  $loan->isExpired();
  $loan->loanExpiresIn();

See the class for information about the methods.

# Publizon user
This sub-module is only used to hook into the login process and to provide
blocks for "my page". It has two utility functions that may be useful outside
the module.

  publizon_user_get_credentials();
  publizon_user_is_logged_in();
