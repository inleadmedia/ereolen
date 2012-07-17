<?php
  /**
   * This template is used to display information about a user with have been
   * found as part of the elib administration interface.
   */
?>
<div class="elib-user-search-resault">
  <h2><?php echo t('User information') ?></h2>
  <div class="form-item">
    <ul>
      <li><strong>UID</strong>: <?php echo $uid ?></li>
      <li><strong>Created</strong>: <?php echo date_format_date($info['user']->created, 'short') ?></li>
      <li><strong>Accessed</strong>: <?php echo date_format_date($info['user']->access, 'short') ?></li>
      <li><strong>Login</strong>: <?php echo date_format_date($info['user']->login, 'short') ?></li>
    </ul>  
  </div>
  <h2><?php echo t('Netsound cart information') ?></h2>
  <div class="form-item">
    <?php if (isset($info['cart']) && !empty($info['cart'])) { ?>
      <ul>
      <?php foreach ($info['cart'] as $object) { ?>
        <li><?php echo l($object->title, 'ting/object/' . $object->id) ?> (<?php echo $object->id ?>)</li>
      <?php }?>
    </ul>
    <?php } else {?>
      <p class="empty-cart"><?php echo t('The users cart is empty') ?></p>
    <?php } ?>
  </div>
</div>