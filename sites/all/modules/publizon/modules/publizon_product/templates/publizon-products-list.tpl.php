<?php
/**
 * @file
 *
 */
?>
<ul class="publizon-products-list">
  <?php foreach ($products as $product): ?>
    <li class="display-book biglist"><?php echo theme('publizon_product', $product, $type); ?></li>
  <?php endforeach; ?>
</ul>