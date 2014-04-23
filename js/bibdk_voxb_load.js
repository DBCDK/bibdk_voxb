(function ($) {

  Drupal.bibdkSetRating = function (voxb) {
    if (voxb.error) {
      $('.bibdk_voxb_tab[data-pid=' + voxb.pid + ']').append(voxb.error);
    }
    else {
      $('.bibdk_voxb_tab[data-pid=' + voxb.pid + ']').html(voxb.markup);
      Drupal.voxb_settings.init(voxb);
    }
  }

    Drupal.bibdkGetRating = function (div) {
        div.find('.rating').last().append('<span class="ajax-progress" style="padding-left:2em; margin-top:-3px"><span class="throbber"></span></span>');
        var pid = $(div).attr('data-pid');
        var request = $.ajax({
            url: Drupal.settings.basePath + 'voxb/ajax/get_rating',
            type: 'POST',
            data: {
                pid: pid
            },
            dataType: 'json',
            success: Drupal.bibdkSetRating
        });
    }

    Drupal.voxbUpdateRating = function (link) {
        var href = Drupal.settings.basePath + link.attr('href');
        // find the div holding the link
        var div = link.closest('.bibdk_voxb_tab');
        // show a throbber

        // do an ajax call. No response is expected
        $.get(href, function (data) {
            if (data.error) {
                data.pid = $(div).attr('data-pid');
                Drupal.bibdkSetRating(data);
            }
            else {
                // update after request is completed
                Drupal.bibdkGetRating(div);
            }
        });
    };



    Drupal.voxb_settings = {
        init: function () {
            // add mouseover
            $('.voxb-rating.rate-enabled .rating').mouseover(function (e) {
                $(this).addClass('star-hover');
            });
            // add mouseout
            $('.voxb-rating.rate-enabled .rating').mouseout(function (e) {
                $(this).removeClass('star-hover');
            });
            // add click
            $('.voxb-rating.rate-enabled .rating').click(function (e) {
                e.preventDefault();
                Drupal.voxbUpdateRating($(this));
            });
        }
    };

    Drupal.behaviors.bibdk_voxb_load = {
        attach: function (context) {
            $('.reviews', context).click(function (e) {
                var href = $(this).attr('href');
                var voxb_tab = $(href).find('.bibdk_voxb_tab');
                Drupal.bibdkGetRating(voxb_tab);
            });
        }
    };
}
    (jQuery));

